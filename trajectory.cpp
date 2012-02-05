/* trajectory.cpp
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
 * Implement trajectory calculation algorithms defined in trajectory.h
 */     

#include "trajectory.h"
#include <cmath>

using namespace std;

/*
 * Some Formulas:
 * 
 * Method one: based on desired angle of entry
 * 
 * alpha = desired angle of entry (positive)
 * v_f = velocity of entry
 * v_0 = velocity of launch
 * g = acceleration due to gravity
 * dx = change in x distance
 * dy = change in y distance (remember to subtract the height of the turret!)
 * theta = angle of launch
 * 
 *                           g * dx^2
 * v_f^2 = ---------------------------------------------
 *           2 * (dy + dx * tan alpha) * (cos alpha)^2
 * 
 *                                            [       g * dx        ]^2
 * v_0^2 = v_f^2 - (2 * g * dx * tan alpha) + [ ------------------- ]
 *                                            [   v_f * cos alpha   ]
 * 
 *                  [    (v_f * sin alpha) - (g * dx) / (v_f * cos alpha)    ]
 * theta = - arctan [  ----------------------------------------------------  ]
 *                  [                   v_f * cos alpha                      ]
 * 
 * Method two: fix launch speed, and just change angle (use last formula above)
 * 
 * Method n: some combination of the above?  something completely different?
 * 
 */
 
double get_entry_velocity(double alpha, double dy, double dx, double g) {
    double num = g * dx * dx;
    double denom = cos(alpha);
    denom *= denom;
    denom *= 2 * (dy + dx * tan(alpha));
    return sqrt(num / denom);
}

double get_launch_velocity(double alpha, double dx, double vf, double g) {
    double ret = vf;
    ret *= vf;
    ret -= 2 * g * dx * tan(alpha);
    double comp = g * dx;
    comp /= vf * cos(alpha);
    comp *= comp;
    return sqrt(ret + comp);
}

double get_launch_angle(double alpha, double dx, double vf, double g) {
    double comp = vf * cos(alpha);
    double num = -g * dx;
    num /= comp;
    num += vf * sin(alpha);
    return -(atan2(num, comp));
}
