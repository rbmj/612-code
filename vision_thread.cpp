#include "vision_thread.h"
#include "612.h"
#include "ranges.h"
#include "vision_alt.h"
#include "update.h"
#include "ports.h"
#include "particle_filter.h"
#include <Vision/AxisCamera.h>
#include <Vision/Threshold.h>
#include <Timer.h>
#include <Task.h>
#include <cstdio>
#include <cstring>
#include <vxWorks.h>
#include <nivision.h>

#include <algorithm>
#include <utility>
#include <fstream>

#if DEBUG_612
const int WRITE_IMAGE = 9;
#endif

//NI function
IMAQ_FUNC int Priv_SetWriteFileAllowed(UINT32 enable); 

void update_targets(vision_thread&, ColorImage&);

vision_thread::vision_thread(AxisCamera& c, vision_processor p) : thread_obj("vision_processing", (FUNCPTR)thread_worker), cam(c) {
    callback = p;
    enabled = true;
    //initialize mutex
    target_lock = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
    //make new thread  
    if (!thread_obj.Start((taskarg_t)this)) {
        std::printf("Vision: Error creating thread.\n");
    }
}

vision_thread::~vision_thread() {
    //destroy other thread
    std::printf("Destroying semaphore\n");
    semDelete(target_lock);
    thread_obj.Stop();
}

int vision_thread::thread_worker(taskarg_t obj) {
    ((vision_thread*)obj)->process_loop();
    //should never be reached
    return 0;
}

void vision_thread::process_loop() 
{
    //this the routine puts us in here.  Do vision processing
    while (true) 
    {
        //YES, I KNOW GOTOS ARE SUPPOSED TO BE EVIL.  I AM USING THEM HERE
        //TO AVOID NEEDLESS SENTINEL VARIABLES.
        
        //You dont need variables for this or gotos
        if (enabled) 
        {
            //get picture from camera
#if DEBUG_612
                Timer process_time;
                process_time.Start();
#endif
            if (camera().IsFreshImage())
            {
                if (!camera().GetImage(&image))
                    perror_612("Cannot Recieve Image From Camera");
                else
                {
                    //do vision processing
                    callback(*this, image);
#if DEBUG_612
                    output_debug_info();
                    //std::printf("Processing took %f sec\n", process_time.Get());
#endif
                }
            }
        }
        Wait(0.05); //should be a trivial wait compared to processing wait
        //wait is here to keep sucking up processing power while polling the
        //camera object
    }
}

void vision_thread::enable() {
    enabled = true;
}

void vision_thread::disable() {
    enabled = false;
}

void vision_thread::lock_targets() {
    semTake(target_lock, WAIT_FOREVER);
}

void vision_thread::release_targets() {
    semGive(target_lock);
}

//returns if thread is locked
bool vision_thread::lock_targets_nowait() {
    if (semTake(target_lock, NO_WAIT) == ERROR) {
        //unsuccessful
        return false;
    }
    else {
        //locked
        return true;
    }
}

vision_targets::vision_targets()
#ifdef VISION_ALT_HEURISTIC
    : bottom_t(bottom_basket), top_t(top_basket), left_t(midleft_basket), right_t(midright_basket) {
#else
{
    target t;
    for (unsigned i = 0; i < numtargets; i++) {
        targets_vec.push_back(t);
    }
#endif 
    registry().register_func(update_helper, (void*)this);
}

vision_targets::~vision_targets() {
    registry().unregister_func(update_helper, (void*)this);
}

void vision_targets::update_helper(void * obj) {
    ((vision_targets*)obj)->update();
}

void vision_targets::update() {
    if (get_vision_thread().lock_targets_nowait()) {
#ifdef VISION_ALT_HEURISTIC
        bottom_t = bottom_basket;
        left_t = midleft_basket;
        right_t = midright_basket;
        top_t = top_basket;
#else
        for (unsigned i = 0; i < numtargets; i++) {
            targets_vec.at(i) = target_arr[i];
        }
#endif
        get_vision_thread().release_targets();
    }
}

#ifdef VISION_ALT_HEURISTIC

target& vision_targets::bottom() {
    return bottom_t;
}

target& vision_targets::top() {
    return top_t;
}

target& vision_targets::left() {
    return left_t;
}

target& vision_targets::right() {
    return right_t;
}

#else

std::vector<target> vision_targets::targets() {
    return targets_vec;
}

#endif

vision_thread& get_vision_thread() {
    static vision_thread t(camera(), update_targets);
    return t;
}

vision_targets& get_targets() {
    static vision_targets t;
    return t;
}

//FORCE INSTANTIATION
vision_thread& VISION_THREAD_INSTANCE = get_vision_thread();
vision_targets& VISION_TARGETS_INSTANCE = get_targets();

//MUST DELETE RETURN VALUE IN CALLER
std::pair<report_vector*, BinaryImage*> do_threshold(ColorImage& image) {
    //Threshold objects
    static Threshold HSL_THOLD(HSL_HMIN, HSL_HMAX, HSL_SMIN, HSL_SMAX, HSL_LMIN, HSL_LMAX);
    static Threshold HSI_THOLD(HSI_HMIN, HSI_HMAX, HSI_SMIN, HSI_SMAX, HSI_IMIN, HSI_IMAX);
    static Threshold HSV_THOLD(HSV_HMIN, HSV_HMAX, HSV_SMIN, HSV_SMAX, HSV_VMIN, HSV_VMAX);
    BinaryImage * result = NULL;
    if (COLOR_MODE == HSL) {
        result = image.ThresholdHSL(HSL_THOLD);
    }
    else if (COLOR_MODE == HSI) {
        result = image.ThresholdHSI(HSI_THOLD);
    }
    else if (COLOR_MODE == HSV) {
        result = image.ThresholdHSV(HSV_THOLD);
    }
    else {
        //color mode undefined.  Fall back to HSL
        result = image.ThresholdHSL(HSL_THOLD);
        perror_612("Invalid Color Mode - Falling Back to HSL");
    }
    if (!result) {
        //we have issues
        perror_612("Threshold Unsuccessful");
//        return std::make_pair<report_vector*, BinaryImage*>(NULL,NULL);
        return std::make_pair((report_vector*)NULL, (BinaryImage*)NULL);
    }
    report_vector * ret = result->GetOrderedParticleAnalysisReports();
    //delete result;//TODO: may be more efficient if we can allocate one binary
                    //image and call imaqColorThreshold.  leaving as is now
                    //cause it's prettier this way and POITROAE.
//    return std::make_pair<report_vector*, BinaryImage*>(ret, result);
    return std::make_pair(ret, result);
}

void do_particle_filter(report_vector * v, Image * img) {
    //filter particles
    particle_filter filt(img);
    v->erase(std::remove_if(v->begin(), v->end(), filt), v->end());
    if (v->size() > 4) { //more than four elements
        v->resize(4); //drop extra elements - get four biggest
    }
}

void write_particles(const char * fname, const report_vector * v) {
    //write particle analyses
    std::fstream o(fname, std::fstream::out | std::fstream::trunc);
    o << "#ifndef REPORT_H_INC\n"
         "#define REPORT_H_INC\n\n"
         "struct Rect {\n"
         "\tint top;\n"
         "\tint left;\n"
         "\tint height;\n"
         "\tint width;\n"
         "};\n\n"
         "struct ParticleAnalysisReport {\n"
         "\tint imageHeight;\n"
         "\tint imageWidth;\n"
         "\tdouble imageTimestamp;\n"
         "\tint particleIndex;\n"
         "\tint center_mass_x;\n"
         "\tint center_mass_y;\n"
         "\tdouble center_mass_x_normalized;\n"
         "\tdouble cneter_mass_y_normalized;\n"
         "\tdouble particleArea;\n"
         "\tRect boundingRect;\n"
         "\tdouble particleToImagePercent;\n"
         "\tdouble particleQuality;\n"
         "};\n\n"
         "ParticleAnalysisReport reports[] = {\n";
    if (v->size()) {
        report_vector::const_iterator it;
        for (it = v->begin(); it < v->end(); it++) {
            o << "\t{\n"
                 "\t\t" << it->imageHeight << ",\n"
                 "\t\t" << it->imageWidth << ",\n"
                 "\t\t" << it->imageTimestamp << ",\n"
                 "\t\t" << it->particleIndex << ",\n"
                 "\t\t" << it->center_mass_x << ",\n"
                 "\t\t" << it->center_mass_y << ",\n"
                 "\t\t" << it->center_mass_x_normalized << ",\n"
                 "\t\t" << it->center_mass_y_normalized << ",\n"
                 "\t\t" << it->particleArea << ",\n"
                 "\t\t{\n"
                 "\t\t\t" << it->boundingRect.top << ",\n"
                 "\t\t\t" << it->boundingRect.left << ",\n"
                 "\t\t\t" << it->boundingRect.height << ",\n"
                 "\t\t\t" << it->boundingRect.width << ",\n"
                 "\t\t},\n"
                 "\t\t" << it->particleToImagePercent << ",\n"
                 "\t\t" << it->particleQuality << ",\n"
                 "\t}";
            if (it + 1 != v->end()) {
                o << ',';
            }
            o << std::endl;
        }
    }
    o << "};\n\n"
         "#endif" << std::endl;
    //PHEW.
    o.close();
}


void update_targets(vision_thread& thread, ColorImage& image) {
    std::pair<report_vector*, BinaryImage*> ptrs = do_threshold(image);
    report_vector * reports = ptrs.first;
    BinaryImage * img = ptrs.second;
#if DEBUG_612
    if (left_joystick.GetRawButton(WRITE_IMAGE)) {
        std::printf("Writing...\n");
        char buffer[64];
        std::sprintf(buffer, "%f_clr.bmp", Timer::GetFPGATimestamp());
        image.Write(buffer);
        int x = std::strlen(buffer);
        buffer[x-6] = 'b';
        buffer[x-5] = 'i';
        buffer[x-4] = 'n';
        img->Write(buffer);
        buffer[x-7] = '.';
        buffer[x-6] = 'h';
        buffer[x-5] = '\0';
        write_particles(buffer, reports);
    }
#endif
    if (!reports) {
        //particle analysis failed.
        if (img) {
            delete img;
        }
        return;
    }
    do_particle_filter(reports, img->GetImaqImage());
    thread.lock_targets();
    target::id_and_process(reports);
    thread.release_targets();
    delete reports; //free vector
    delete img; //free image
}
