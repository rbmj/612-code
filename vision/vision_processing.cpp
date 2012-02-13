/* vision/vision_processing.cpp
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
 * Entry points for vision processing: distance and angle from image
 */
 
/*
 * Calculated angles are relative to perpendicular.
 */

#include <vector>
#include <cmath>
#include <Vision/ColorImage.h>
#include <Vision/BinaryImage.h>
#include <Vision2009/VisionAPI.h>
#include <nivision.h>

#include "vision_processing.h"
#include "../ports.h"
#include "../ranges.h"
#include "../trajectory.h"
#include "../visionalg.h"

using namespace vision_processing;

typedef struct particle_rect_struct {
    int top;    //Location of the top edge of the rectangle.
    int left;   //Location of the left edge of the rectangle.
    int height; //Height of the rectangle.
    int width;  //Width of the rectangle.
} particle_rect;

//constants
HSLImage old_image;

BinaryImage* image_mask;
vector<ParticleAnalysisReport>* targets;

BinaryImage* get_image_mask(ColorImage*);
vector<ParticleAnalysisReport>* get_image_targets(BinaryImage*);

double inline degrees_from_ratio(double); // ratio: width/height
double inline radians_from_ratio(double);
double inline distance_from_height(int);
double inline deviation_from_angle(double);

ColorImage* vision_processing::get_image() {
    camera().GetImage(&old_image);
    return &old_image;
}

ColorImage* vision_processing::get_old_image() {
    return &old_image;
}

unsigned int vision_processing::determine_aim_target() {
    // TODO make it do stuff
    return 0;
}

vector<double> vision_processing::get_distance() {
    vector<double> distance;
    for(unsigned int i = 0; i < targets->size(); i++) {
        ParticleAnalysisReport target = targets->at(i);
        Rect target_rect=target.boundingRect;
        int height = target_rect.height;
        int width = target_rect.width;
        double ratio = 1.0 * width/height;
        double image_degrees = degrees_from_ratio(ratio);
		double ground_distance = distance_from_height(height) + deviation_from_angle(image_degrees);
        distance.push_back(ground_distance);
    }
    return distance;
}

vector<double> vision_processing::get_degrees() {
    vector<double> degrees;
    for(unsigned int i = 0; i < targets->size(); i++) {
        ParticleAnalysisReport target = targets->at(i);
        Rect target_rect = target.boundingRect;
        int height = target_rect.height;
        int width = target_rect.width;
        double ratio = 1.0 * width/height;
        double image_degrees = degrees_from_ratio(ratio);
        degrees.push_back(image_degrees);
    }
    return degrees;
}

vector<double> vision_processing::get_radians() {
    vector<double> degrees = get_degrees();
    vector<double> radians;
    for(unsigned int i = 0; i< degrees.size(); i++) {
        radians.push_back(deg2rad(degrees[i]));
    }
    return radians;
}

BinaryImage* get_image_mask(ColorImage* image) {
    if(image == NULL) {
        return image_mask;
    }
    if (COLOR_MODE == HSV) {
        image_mask = image->ThresholdHSV(HSV_HMIN, HSV_HMAX, HSV_SMIN, HSV_SMAX, HSV_VMIN, HSV_VMAX);
    }
    else if(COLOR_MODE == HSI) {
        image_mask = image->ThresholdHSI(HSI_HMIN, HSI_HMAX, HSI_SMIN, HSI_SMAX, HSI_IMIN, HSI_IMAX);
    }
    else { // HSL is implied (not assumed)
        image_mask = image->ThresholdHSL(HSL_HMIN, HSL_HMAX, HSL_SMIN, HSL_SMAX, HSL_LMIN, HSL_LMAX);
    }
    return image_mask;
}

vector<ParticleAnalysisReport>* get_image_targets(BinaryImage* image) {
    vector<ParticleAnalysisReport>* targets = new vector<ParticleAnalysisReport>();
    if(image == NULL) {
        return targets;
    }
    vector<ParticleAnalysisReport>* particles=image->GetOrderedParticleAnalysisReports();
    for(unsigned int i = 0; i < particles->size(); i++) {
        ParticleAnalysisReport particle = particles->at(i);
        double particle_area = particle.particleArea;
        if(particle_area > PARTICLE_AREA_MIN && particle_area <= PARTICLE_AREA_MAX) {
            if(targets->size() >= 4) {
                // TODO change min and max
                //      call function again
                //      if depth is more than 2
                //      explode
                break;
            }
            targets->push_back(particle);
        }
    }
    delete particles;
    return targets;
}

void vision_processing::update() {
    if(!camera().IsFreshImage()) {
        return;
    }
    delete image_mask;
    image_mask = get_image_mask(&old_image);
    delete targets;
    targets = get_image_targets(image_mask);
}

//TODO: Someone else (Jeff?) sanity check this and make sure it's right.  I tried to copy your logic
//from above.
double get_distance_from_report(const ParticleAnalysisReport& report) {
    double ratio = ((double)(report.boundingRect.width))/(report.boundingRect.height);
    double degrees = degrees_from_ratio(ratio);
    double dist = distance_from_height(report.boundingRect.height) + deviation_from_angle(degrees);
    return dist;
}

double get_height_offset_from_report(const ParticleAnalysisReport& r, double dist) {
    //meant to be called once you have dist from get_distance_from_report
    //this way we don't need to have target detection
    double theta = angle_offset(RESOLUTION().Y()/2 - r.center_mass_y, RESOLUTION().Y(), FOV().Y()); 
    return std::tan(theta)*dist;
}

double inline degrees_from_ratio(double ratio) {
    //a quadratic regression.  Fits rather well.
    //aspect ratio is constant 4:3, so no need to adjust ratio
    return (-94.637 * ratio * ratio) + (119.86 * ratio) + 9.7745;
}

double inline radians_from_ratio(double ratio) {
    return deg2rad(degrees_from_ratio(ratio));
}

double inline distance_from_height(int height) {
    //magic numbers are gross but they do the job...
    //a tad worried about the -0.8.  Not sure what this will do at close distances
#ifdef RESOLUTION_640_480
    //no need to do anything
#elif defined RESOLUTION_320_240
    height *= 2; //adjust for resolution
#elif defined RESOLUTION_160_120
    height *= 4;
#endif
    return ((1277.686246075*(1/height)) - 0.8265433113);
}

double inline deviation_from_angle(double angle) {
	return ((0.0188*angle) + 0.017);
}
