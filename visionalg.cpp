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

#include "visionalg.h"

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
 
#include <cmath>

const double CAM_FOV = 0.820304748; //47 degrees
const int AR_X = 4; //4:3 aspect
const int AR_Y = 3;
const int RES_X = 320;
const int RES_Y = 240;

double angle_offset(int offset, int total, double FOV) {
    double c = 2 * std::sin(FOV / 2);
    c *= offset;
    c /= total;
    return std::asin(c);
}

double fov_axis(int axis_one, int axis_two, double total) {
    //returns field of view along axis_one with aspect ratio one:two
    double val = axis_one / std::sqrt((axis_one * axis_one) + (axis_two * axis_two));
    val *= std::tan(total);
    return std::atan(val);
}

camera_fov::camera_fov(double fov, aspect_ratio r) : field_of_view(fov), ratio(r) {
    field_of_view_x = fov_axis(ratio.X(), ratio.Y(), field_of_view);
    field_of_view_y = fov_axis(ratio.Y(), ratio.X(), field_of_view);
}

camera_fov fov_obj(CAM_FOV, aspect_ratio(AR_X, AR_Y));

const camera_fov& FOV() {
    return fov_obj;
}

aspect_ratio res_obj(RES_X, RES_Y);

const aspect_ratio& RESOLUTION() {
    return res_obj;
}

aspect_ratio::aspect_ratio(int xv, int yv) : x(xv), y(yv) {}
 
