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
#include "../ranges.h"
#include "../trajectory.h"

using namespace vision_processing;

//constants
ColorImage* old_image;

double inline degrees_from_ratio(double); // ratio: width/height
double inline radians_from_ratio(double);
double inline distance_from_height(int);
unsigned int get_image_height(BinaryImage*, ParticleAnalysisReport);

ColorImage* vision_processing::get_image() {
    if(camera().IsFreshImage()) {
        camera().GetImage(old_image);
    } 
    return get_old_image();
}

ColorImage* vision_processing::get_old_image() {
    return old_image;
}

BinaryImage* vision_processing::get_image_mask(ColorImage* image) {
    BinaryImage* imageMask;
    if (COLOR_MODE == HSV) {
        imageMask = image->ThresholdHSV(HSV_HMIN, HSV_HMAX, HSV_SMIN, HSV_SMAX, HSV_VMIN, HSV_VMAX);
    }
    else if(COLOR_MODE == HSI) {
        imageMask = image->ThresholdHSI(HSI_HMIN, HSI_HMAX, HSI_SMIN, HSI_SMAX, HSI_IMIN, HSI_IMAX);
    }
    else { // HSI is implied (not assumed)
        imageMask = image->ThresholdHSL(HSL_HMIN, HSL_HMAX, HSL_SMIN, HSL_SMAX, HSL_LMIN, HSL_LMAX);
    }
    return imageMask;
}

vector<ParticleAnalysisReport> vision_processing::get_image_targets(BinaryImage* image) {
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

unsigned int vision_processing::determine_aim_target_from_image(ColorImage*) {
    return determine_aim_target(get_image_targets(get_image_mask(get_image())));
}

unsigned int vision_processing::determine_aim_target(vector<ParticleAnalysisReport>) {
    // TODO make it do stuff
    return 0;
}

vector<double> vision_processing::get_distance() {
    return get_distance_from_image(get_image());
}

vector<double> vision_processing::get_distance_from_image(ColorImage* image) {
    BinaryImage* image_mask = get_image_mask(get_image());
    vector<ParticleAnalysisReport> targets = get_image_targets(image_mask);
    vector<double> distance;
    for(unsigned int i = 0; i < targets.size(); i++) {
        ParticleAnalysisReport target = targets[i];
		unsigned int height = get_image_height(image_mask, target);
		double ground_distance = distance_from_height(height);
        distance.push_back(ground_distance);
    }
    return distance;
}

vector<double> vision_processing::get_degrees() {
    return get_degrees_from_image(get_image());
}

vector<double> vision_processing::get_degrees_from_image(ColorImage* image) {
    vector<ParticleAnalysisReport> targets = get_image_targets(get_image_mask(get_image()));
    vector<double> degrees;
    for(unsigned int i = 0; i < targets.size(); i++) {
        ParticleAnalysisReport target = targets[i];
        int height = target.imageHeight;
        int width = target.imageWidth;
        double ratio = 1.0*width/height;
        degrees.push_back(degrees_from_ratio(ratio));
    }
    return degrees;
}

vector<double> vision_processing::get_radians_from_image(ColorImage* image) {
    vector<double> degrees = get_degrees_from_image(image);
    vector<double> radians;
    for(unsigned int i = 0; i< degrees.size(); i++) {
        radians.push_back(deg2rad(degrees[i]));
    }
    return radians;
}

double inline degrees_from_ratio(double ratio) {
    return (-94.637 * pow(ratio, 2)) + (119.86 * ratio) + 9.7745;
}

double inline radians_from_ratio(double ratio) {
    return deg2rad(degrees_from_ratio(ratio));
}

double inline distance_from_height(int height) {
    return 1532.1932574739 * (pow(height, -1.0541299046));
}

unsigned int get_image_height(BinaryImage* image, ParticleAnalysisReport target) {
    // return black_magic();
    return 0;
}
