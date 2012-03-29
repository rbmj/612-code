/* visionalg.cpp
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
 * Implement algorithms for vision processing.
 */

#include <cmath>
#include <vector>
#include <Vision/AxisCameraParams.h>
#include "visionalg.h"
#include "ports.h"
#include "vision_alt.h"

/* A few algorithms:
 * 
 * FOV given aspect ratio x:y
 * 
 * theta: diagonal angle covered by camera image
 * theta_x: angle covered horizontally
 * theta_y: angle covered vertically
 * 
 *                  [           x                     ]
 * theta_x = arctan [ --------------------- tan theta ]
 *                  [   sqrt( x^2 + y^2 )             ]
 * 
 *                  [           y                     ]
 * theta_x = arctan [ --------------------- tan theta ]
 *                  [   sqrt( x^2 + y^2 )             ]
 */
 
/*
 * Angular Offset of a Point:
 * 
 *                [   2 sin ( FOV / 2) * r_offset   ]
 * theta = arcsin [ ------------------------------- ]
 *                [            r_total              ]
 */

#ifdef AXIS_CAMERA_M1011
const double CAM_FOV = 0.820304748; //47 degrees, Axis M1011
#elif defined AXIS_CAMERA_206
const double CAM_FOV = 0.959931089; //55 degrees, Axis 206
#else
#error Invalid Camera Specified
#endif

const int AR_X = 4; //4:3 aspect
const int AR_Y = 3;

#ifdef RESOLUTION_640_480
const int RES_X = 640;
const int RES_Y = 480;
const AxisCameraParams::Resolution_t axis_resolution = AxisCameraParams::kResolution_640x480;
#elif defined RESOLUTION_320_240
const int RES_X = 320;
const int RES_Y = 240;
const AxisCameraParams::Resolution_t axis_resolution = AxisCameraParams::kResolution_320x240;
#elif defined RESOLUTION_160_120
const int RES_X = 160;
const int RES_Y = 120;
const AxisCameraParams::Resolution_t axis_resolution = AxisCameraParams::kResolution_160x120;
#else
#error Invalid Resolution Specified
#endif

camera_fov fov_obj(CAM_FOV, aspect_ratio(AR_X, AR_Y));
aspect_ratio res_obj(RES_X, RES_Y);

double fov_axis(int axis_one, int axis_two, double total) {
    //returns field of view along axis_one with aspect ratio one:two
    double val = axis_one / std::sqrt((axis_one * axis_one) + (axis_two * axis_two));
    val *= std::tan(total);
    return std::atan(val);
}

double angle_offset(int offset, int total, double FOV) {
    double c = 2 * std::sin(FOV / 2);
    c *= offset;
    c /= total;
    return std::asin(c);
}

void init_camera() {
    if (camera().GetResolution() != axis_resolution) {
        camera().WriteResolution(axis_resolution);
    }
}
aspect_ratio::aspect_ratio(int xv, int yv) : x(xv), y(yv) {}

camera_fov::camera_fov(double fov, aspect_ratio r) : field_of_view(fov), ratio(r) {
    field_of_view_x = fov_axis(ratio.X(), ratio.Y(), field_of_view);
    field_of_view_y = fov_axis(ratio.Y(), ratio.X(), field_of_view);
}

const camera_fov& FOV() {
    return fov_obj;
}

const aspect_ratio& RESOLUTION() {
    return res_obj;
}

target* ascertain_primary_target() {
    vision_targets& ts = get_targets();
    target* t;
#ifdef VISION_ALT_ADHOC
    //adhoc
    std::vector<target> t_vec = ts.targets();
    t = NULL;
    for (unsigned i = 0; i < numtargets && t_vec[i].valid(); i++) {
        if (t) {
            if (t_vec[i].height() > t->height()) {
                t = &(t_vec[i]);
            }
        }
        else {
            t = &(t_vec[i]);
        }
    }
#else
    //heuristic
    if (ts.top().valid()) {
        t = &(ts.top());
    }
    else if (ts.left().valid()) {
        t = &(ts.left());
    }
    else if (ts.right().valid()) {
        t = &(ts.right());
    }
    else if (ts.bottom().valid()) {
        t = &(ts.bottom());
    }
    else {
        t = NULL;
    }
#endif
    return t;
}
