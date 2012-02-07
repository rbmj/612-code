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
#include "vision_processing.h"
#include "../ports.h"
//#include "../ranges.h" no such file or directory - see below
#include "../trajectory.h"

enum COLOR_MODE_T {
    HSL,
    HSI,
    HSV
};

//set these to change search color
const COLOR_MODE_T COLOR_MODE = HSV;
const int COLOR_FIRST_MIN  = 0; //H   - min
const int COLOR_FIRST_MAX  = 0; //    - max
const int COLOR_SECOND_MIN = 0; //S   - min
const int COLOR_SECOND_MAX = 0; //    - max
const int COLOR_THIRD_MIN  = 0; //LIV - min
const int COLOR_THIRD_MAX  = 0; //    - max

//configure the particle
const int PARTICLE_AREA_MIN = 500;
const int PARTICLE_AREA_MAX = 10000;

//constants
ColorImage* old_image;

double degrees_from_ratio(double); // ratio: width/height
double radians_from_ratio(double);
vector<unsigned int> CornerFind(vector<ParticleAnalysisReport>);

ColorImage* get_image() {
    if(camera().IsFreshImage()) {
        camera().GetImage(old_image);
    } 
    return get_old_image();
}

ColorImage* get_old_image() {
    return old_image;
}

BinaryImage* get_image_mask(ColorImage* image) {
    BinaryImage* imageMask;
    if (COLOR_MODE == HSV) {
        imageMask = image->ThresholdHSV(COLOR_FIRST_MIN, COLOR_FIRST_MAX, COLOR_SECOND_MIN, COLOR_SECOND_MAX, COLOR_THIRD_MIN, COLOR_THIRD_MAX);
    }
    else if (COLOR_MODE == HSI) {
        imageMask = image->ThresholdHSI(COLOR_FIRST_MIN, COLOR_FIRST_MAX, COLOR_SECOND_MIN, COLOR_SECOND_MAX, COLOR_THIRD_MIN, COLOR_THIRD_MAX);
    }
    else { // assume HSL
        imageMask = image->ThresholdHSL(COLOR_FIRST_MIN, COLOR_FIRST_MAX, COLOR_SECOND_MIN, COLOR_SECOND_MAX, COLOR_THIRD_MIN, COLOR_THIRD_MAX);
    }
    return imageMask;
}

vector<ParticleAnalysisReport> vision_processing(BinaryImage* image) {
    vector<ParticleAnalysisReport>* particles = image->GetOrderedParticleAnalysisReports();
    vector<ParticleAnalysisReport> targets;
    for(unsigned int i = 0; i < particles->size(); i++) {
        ParticleAnalysisReport particle = particles->at(i);
        double particle_area = particle.particleArea;
        if(particle_area > PARTICLE_AREA_MIN && particle_area <= PARTICLE_AREA_MAX) {
            if(targets.size() >= 4) {
                // TODO change min and max
                //      call function again
                //      if depth is more than 2
                //      explode
                break;
            }
            targets.push_back(particle);
        }
    }
    return targets;
}

unsigned int determine_aim_target_from_image(ColorImage*) {
    return determine_aim_target(get_image_targets(get_image_mask(get_image())));
}

unsigned int determine_aim_target(vector<ParticleAnalysisReport>) {
    // TODO make it do stuff
    return 0;
}

vector<double> get_distance() {
    return get_distance_from_image(get_image());
}

vector<double> vision_processing::get_distance_from_image(ColorImage* image) {
    vector<ParticleAnalysisReport> targets = get_image_targets(get_image_mask(get_image()));
    vector<double> distance;
    for(unsigned int i = 0; i < targets.size(); i++) {
		vector<unsigned int> corners_y = CornerFind(targets);
		/*-----Corners--------*/
		//0,1
		//2,3
		/*--------------------*/
		unsigned int top_midpoint_y = (corners_y.at(0) + corners_y.at(1))/2 ;
		unsigned int bottom_midpoint_y = (corners_y.at(2) + corners_y.at(3))/2 ;
		unsigned int height = top_midpoint_y - bottom_midpoint_y;
		unsigned double ground_distance = 1532.1932574739*(pow(height,-1.0541299046));
        distance.push_back(ground_distance);
    }
    return distance;
}

vector<unsigned int> CornerFind(vector<ParticleAnalysisReport> targets) {
	vector<unsigned int> corners;
	//Add Black Magic Here
	return corners;
}

vector<double> get_degrees() {
    return get_degrees_from_image(get_image());
}

vector<double> get_degrees_from_image(ColorImage* image) {
    vector<ParticleAnalysisReport> targets = get_image_targets(get_image_mask(get_image()));
    vector<double> degrees;
    for(unsigned int i = 0; i < targets.size(); i++) {
        int height = targets[i].imageHeight;
        int width = targets[i].imageWidth;
        double ratio = 1.0*width/height;
        degrees.push_back(degrees_from_ratio(ratio));
    }
    return degrees;
}

vector<double> get_radians_from_image(ColorImage* image) {
    vector<double> degrees = get_degrees_from_image(image);
    vector<double> radians;
    for(unsigned int i = 0; i< degrees.size(); i++) {
        radians.push_back(deg2rad(degrees[i]));
    }
    return radians;
}

double degrees_from_ratio(double ratio) {
    return (-94.637*pow(ratio,2)) + (119.86*ratio) + 9.7745;
}

double radians_from_ratio(double ratio) {
    return deg2rad(degrees_from_ratio(ratio));
}
