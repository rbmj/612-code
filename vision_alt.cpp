/* vision_alt.cpp
 *
 * Copyright (c) 2011, 2012 Chantilly Robotics <chantilly612@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * An alternative implementation of image processing
 */

#include "vision_alt.h"
#include "ranges.h"
#include "update.h"
#include "ports.h"
#include "visionalg.h"
#include "612.h"
#include "vision/vision_processing.h"

#include <nivision.h>
#include <Vision/Threshold.h>
#include <Vision/AxisCamera.h>
#include <Vision/HSLImage.h>
#include <Vision/ColorImage.h>
#include <Vision/BinaryImage.h>
#include <Vision2009/VisionAPI.h>

#include <vector>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

enum VISION_ALGORITHM {
    TRIGONOMETRIC,
    REGRESSION
};

//note: only REGRESSION will work if the target detection doesn't work well :/
const VISION_ALGORITHM ALGORITHM = TRIGONOMETRIC;
//note: if VISION_ALGORITHM_ADHOC is enabled then REGRESSION will be selected
//regardless of what you put here.

/* Remember that y increases as you go DOWN the image!! */

const double robot_height = 31.0/12; //TODO: Replace this with a real #(feet)!

#ifdef VISION_ALT_HEURISTIC
//heights in feet:
//TODO: add in the height to the center of the particle!
target bottom_basket(   22.88/12 );
target midleft_basket(  55.88/12 );
target midright_basket( 55.88/12 );
target top_basket(      92.88/12 );
#elif defined VISION_ALT_ADHOC
const unsigned numtargets = 4;
target target_arr[numtargets];
#endif

//Threshold objects
Threshold HSL_THOLD(HSL_HMIN, HSL_HMAX, HSL_SMIN, HSL_SMAX, HSL_LMIN, HSL_LMAX);
Threshold HSI_THOLD(HSI_HMIN, HSI_HMAX, HSI_SMIN, HSI_SMAX, HSI_IMIN, HSI_IMAX);
Threshold HSV_THOLD(HSV_HMIN, HSV_HMAX, HSV_SMIN, HSV_SMAX, HSV_VMIN, HSV_VMAX);

HSLImage target::image; //The only non-rgb raw image type available

void output_debug_info_target(const char * n, const target& t) {
    if (t.valid()) {
        std::printf("Target %s found:\n"
                    "\tdistance = %g\n"
                    "\tx_offset = %i\n"
                    "\theight   = %g\n",
                    n, t.distance(), t.x_offset(), t.height());
        //
    }
    else {
        std::printf("Target %s not found.\n", n);
    }
}

void output_debug_info() {
#ifdef VISION_ALT_HEURISTIC
    output_debug_info_target("bottom_basket", bottom_basket);
    output_debug_info_target("midleft_basket", midleft_basket);
    output_debug_info_target("midright_basket", midright_basket);
    output_debug_info_target("top_basket", top_basket);
#elif defined VISION_ALT_ADHOC
    for (unsigned i = 0; i < numtargets; i++) {
        output_debug_info_target("generic", target_arr[i]);
    }
#endif
}

/* update logic: */

void target::update_target_arr(void * ignored) {
    if (!camera().IsFreshImage()) {
        return; //no new image for us to process
    }
    if (!camera().GetImage(&image)) {
        perror_612("Cannot Recieve Image From Camera");
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
        perror_612("Invalid Color Mode - Falling Back to HSL");
    }
    if (!result) {
        //we have issues
        perror_612("Threshold Unsuccessful");
        return;
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
#ifdef VISION_ALT_HEURISTIC
    //identify target_arr, and update the values there
    id_and_process(reports);
#elif defined VISION_ALT_ADHOC
    //give each target a particle :)
    if (numtargets < reports->size()) {
        //should NEVER be true
        for (unsigned i = 0; i < numtargets; i++) {
            target_arr[i].update_data_with_report(reports->at(i));
        }
    }
    else {
        for (unsigned i = 0; i < reports->size(); i++) {
            target_arr[i].update_data_with_report(reports->at(i));
        }
        for (unsigned i = reports->size(); i < numtargets; i++) {
            target_arr[i].m_valid = false;
        }
    }
#endif
    delete reports; //free vector
    if (DEBUG_612) {
        if (std::rand() % 5 == 0) { //make it so it outputs 20% of the time
            output_debug_info();
        }
    }
}

#ifdef VISION_ALT_HEURISTIC

void target::three_targets_alignx(const ParticleAnalysisReport& aligna,
                                  const ParticleAnalysisReport& alignb,
                                  const ParticleAnalysisReport& unalign)
{
    //aligna and alignb are vertically aligned
    if (aligna.center_mass_y > alignb.center_mass_y) {
        //aligna is bottom, and alignb is top
        bottom_basket.update_data_with_report(aligna);
        top_basket.update_data_with_report(alignb);
    }
    else {
        //alignb is bottom, and aligna is top
        bottom_basket.update_data_with_report(alignb);
        top_basket.update_data_with_report(aligna);
    }
    if (unalign.center_mass_x < (aligna.center_mass_x + alignb.center_mass_x) / 2) {
        //unalign is midleft
        midleft_basket.update_data_with_report(unalign);
    }
    else {
        //unalign is midright
        midright_basket.update_data_with_report(unalign);
    }
}

void target::three_targets_aligny(const ParticleAnalysisReport& aligna,
                                  const ParticleAnalysisReport& alignb,
                                  const ParticleAnalysisReport& unalign)
{
    //aligna and alignb are horizontally aligned
    if (aligna.center_mass_x < alignb.center_mass_x) {
        //aligna is midleft, and alignb is midright
        midleft_basket.update_data_with_report(aligna);
        midright_basket.update_data_with_report(alignb);
    }
    else {
        //alignb is midleft, and aligna is midright
        midleft_basket.update_data_with_report(alignb);
        midright_basket.update_data_with_report(aligna);
    }
    if (unalign.center_mass_y < (aligna.center_mass_y + alignb.center_mass_y) / 2) {
        //unalign is top
        top_basket.update_data_with_report(unalign);
    }
    else {
        //unalign is bottom
        bottom_basket.update_data_with_report(unalign);
    }
}

void target::id_and_process(report_vector * reports) {
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
        
        int x0_1, x1_2, x0_2;
        x0_1 = std::abs(reports->at(0).center_mass_x - reports->at(1).center_mass_x);
        x1_2 = std::abs(reports->at(1).center_mass_x - reports->at(2).center_mass_x);
        x0_2 = std::abs(reports->at(0).center_mass_x - reports->at(2).center_mass_x);
        
        int y0_1, y1_2, y0_2;
        y0_1 = std::abs(reports->at(0).center_mass_y - reports->at(1).center_mass_y);
        y1_2 = std::abs(reports->at(1).center_mass_y - reports->at(2).center_mass_y);
        y0_2 = std::abs(reports->at(0).center_mass_y - reports->at(2).center_mass_y);
        
        int xmin = std::min(std::min(x0_1, x0_2), x1_2);
        int ymin = std::min(std::min(y0_1, y0_2), x1_2);
        
        if (xmin < ymin) {
            //vertically aligned, therefore two of them are top and bottom
            if (xmin == x0_1) {
                three_targets_alignx(reports->at(0), reports->at(1), reports->at(2));
            }
            else if (xmin == x0_2) {
                three_targets_alignx(reports->at(0), reports->at(2), reports->at(1));
            }
            else if (xmin == x1_2) {
                three_targets_alignx(reports->at(1), reports->at(2), reports->at(0));
            }
        }
        else {
            //horizontally aligned, therefore two of them are left and right
            if (ymin == y0_1) {
                three_targets_aligny(reports->at(0), reports->at(1), reports->at(2));
            }
            else if (ymin == y0_2) {
                three_targets_aligny(reports->at(0), reports->at(2), reports->at(1));
            }
            else if (ymin == y1_2) {
                three_targets_aligny(reports->at(1), reports->at(2), reports->at(0));
            }
        }
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
        midright_basket.update_data_with_report(*(reports->begin()));
    }
    else {
        //shouldn't happen
        perror_612("Too Many Particles.  Aborting.");
    }  
}

#endif

/*
 * Distance - given COM_Y of the backboard
 * 
 * COM_Y ranges 0 (top) to RESOLUTION().Y() (bottom)
 * H is the height (feet) between camera and center of backboard
 * 
 * theta = angle_offset(RESOLUTION().Y()/2 - COM_Y, RESOLUTION().Y(), FOV());
 * DX = H/std::tan(theta);
 */

double get_distance_TRIG(const ParticleAnalysisReport& r, double height) {
    double theta = angle_offset(RESOLUTION().Y()/2 - r.center_mass_y, RESOLUTION().Y(), FOV().Y());
    return (height/std::tan(theta));
}
 
void target::update_data_with_report(const ParticleAnalysisReport & r) {
    m_valid = true;
#ifdef VISION_ALT_HEURISTIC
    double v_offset = m_height - robot_height;
    if (ALGORITHM == TRIGONOMETRIC) {
        m_distance = get_distance_TRIG(r, v_offset);
    }
    else if (ALGORITHM == REGRESSION) {
        m_distance = vision_processing::get_distance_from_report(r);
    }
#elif defined VISION_ALT_ADHOC
    m_distance = vision_processing::get_distance_from_report(r);
    double v_offset = vision_processing::get_height_offset_from_report(r, m_distance);
    m_height = v_offset + robot_height;
#endif
    m_x_offset = r.center_mass_x - (RESOLUTION().X()/2);
}
    
