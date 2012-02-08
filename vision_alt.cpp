#include "vision_alt.h"
#include "ranges.h"
#include "update.h"
#include "ports.h"
#include "visionalg.h"

#include <nivision.h>
#include <Threshold.h>
#include <Vision/AxisCamera.h>
#include <Vision/HSLImage.h>
#include <Vision/ColorImage.h>
#include <Vision/BinaryImage.h>
#include <Vision2009/VisionAPI.h>

#include <vector>
#include <cmath>

/* Remember that y increases as you go DOWN the image!! */

const double robot_height = 31.0/12; //TODO: Replace this with a real #(feet)!

//heights in feet:
target bottom_basket(   22.88/12 );
target midleft_basket(  55.88/12 );
target midright_basket( 55.88/12 );
target top_basket(      92.88/12 );

//Threshold objects
Threshold HSL_THOLD(HSL_HMIN, HSL_HMAX, HSL_SMIN, HSL_SMAX, HSL_LMIN, HSL_LMAX);
Threshold HSI_THOLD(HSI_HMIN, HSI_HMAX, HSI_SMIN, HSI_SMAX, HSI_IMIN, HSI_IMAX);
Threshold HSV_THOLD(HSV_HMIN, HSV_HMAX, HSV_SMIN, HSV_SMAX, HSV_VMIN, HSV_VMAX);

HSLImage target::image; //The only non-rgb raw image type available

/* Helper functions for this file: */

#if 0
//change the 0 to a 1 to enable :)

class static_init_class {
public:
    static_init_class() {
        registry.register_func(target::update_targets, NULL);
    }
    ~static_init_class() {
        registry.unregister_func(target::update_targets, NULL);
    }
};

static_init_class static_init_class_obj; //just so it will run the ctor on startup

#endif

/* update logic: */

void target::update_targets(void * ignored) {
    if (!camera().IsFreshImage()) {
        return; //no new image for us to process
    }
    if (!camera().GetImage(&image)) {
        //error getting image from camera
        return;
    }
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
        //probably should output an error message
    }
    if (!result) {
        //we have issues
        return; //error?
    }
    report_vector * reports = result->GetOrderedParticleAnalysisReports();
    if (!reports) {
        //particle analysis failed. shouldn't happen, but free memory and quit.
        delete result;
        return;
    }
    //done with the binary image, can return that memory
    delete result; //TODO: may be more efficient if we can allocate one binary
                   //image and call imaqColorThreshold.  leaving as is now
                   //cause it's prettier this way and POITROAE.
    if (reports->size() > 4) { //more than four elements
        reports->resize(4); //drop extra elements - get four biggest
    }
    //filter particles according to size
    while (reports->size() && reports->back().particleArea > (double)PARTICLE_AREA_MIN) {
        reports->pop_back();
    }
    //identify targets, and update the values there
    id_and_process(reports);
    delete reports; //free vector
}

void target::id_and_process(const report_vector * reports) {
    //set all valid flags to false for now.  Call update_data_with_report()
    //to make it valid later.
    bottom_basket.m_valid = false;
    midleft_basket.m_valid = false;
    midright_basket.m_valid = false;
    top_basket.m_valid = false;
    
    if (reports->size() == 0) {
        //no particles
    }
    else if (reports->size() == 1) {
        //guess bottom basket
        //TODO: Probably should be a little more intelligent about this
        //check based on position in image? esp y pos cause that doesn't change
        top_basket.update_data_with_report(reports->front());
    }
    else if (reports->size() == 2) {
        //guess that it's bottom basket and one side.
        //TODO: Probably should be a little more intelligent about this too
        
        if (reports->front().center_mass_y > reports->back().center_mass_y) {
            //front() is bottom basket
            bottom_basket.update_data_with_report(reports->front());
            if (reports->front().center_mass_x < reports->back().center_mass_x) {
                //back() is mid-right basket
                midright_basket.update_data_with_report(reports->back());
            }
            else {
                //back() is mid-left basket
                midleft_basket.update_data_with_report(reports->back());
            }
        }
        else {
            //back() is bottom basket
            bottom_basket.update_data_with_report(reports->back());
            if (reports->front().center_mass_x < reports->back().center_mass_x) {
                //front() is mid-left basket
                midleft_basket.update_data_with_report(reports->front());
            }
            else {
                //front() is mid-right basket
                midright_basket.update_data_with_report(reports->back());
            }
        }
    }
    else if (reports->size() == 3) {
        //three on screen.  Check if a line through two of the baskets is approx
        //parallel to an axis.  If it's the x axis, they're the mid-level
        //baskets.  If it's the y axis, they're the top/bottom baskets
        
        //TODO: Implement
    }
    else if (reports->size() == 4) {
        //all baskets on screen.
        //pick lowest for bottom, highest for top, furthest left for midleft,
        //and the last on is midright.
        report_vector::iterator chosen;
        double cmpval = 0.0; //further up than everything
        //find bottom
        for (report_vector::iterator i = reports->begin(); i < reports->end(); ++i) {
            if (i->center_mass_y > cmpval) {
                chosen = i;
            }
        }
        bottom_basket.update_data_with_report(*chosen);
        reports->erase(chosen);
        //find top
        cmpval = (double)RESOLUTION().Y(); //further down than everything
        for (report_vector::iterator i = reports->begin(); i < reports->end(); ++i) {
            if (i->center_mass_y < cmpval) {
                chosen = i;
            }
        }
        top_basket.update_data_with_report(*chosen);
        reports->erase(chosen);
        //find left
        cmpval = (double)RESOLUTION().X();
        for (report_vector::iterator i = reports->begin(); i < reports->end(); ++i) {
            if (i->center_mass_x < cmpval) {
                chosen = i;
            }
        }
        midleft_basket.update_data_with_report(*chosen);
        reports->erase(chosen);
        //should only be one element left in vector
        //right
        midright_basket.update_data_with_report(reports->begin());
    }    
}

/*
 * Distance - given COM_Y of the backboard
 * 
 * COM_Y ranges 0 (top) to RESOLUTION().Y() (bottom)
 * H is the height (feet) between camera and center of backboard
 * 
 * theta = angle_offset(RESOLUTION().Y()/2 - COM_Y, RESOLUTION().Y(), FOV());
 * DX = H/std::tan(theta);
 */
 
void target::update_data_with_report(const ParticleAnalysisReport & r) {
    m_valid = true;
    double v_offset = m_height - robot_height;
    double theta = angle_offset(RESOLUTION().Y()/2 - r.center_mass_y, RESOLUTION().Y(), FOV().Y());
    m_distance = m_height/std::tan(theta);
    Rect rect = r.boundingRect;
    m_x_offset = rect.left + (rect.width/2);
    m_x_offset -= RESOLUTION().X()/2; //move to center
}
    
