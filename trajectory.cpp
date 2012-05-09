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

#include <cmath>
#include "trajectory.h"
#include "vision_alt.h"
#include "shooter.h"

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

trajectory calculate_trajectory_entryangle(const target& t, double a, double g) {
    trajectory ret;
    double vf = get_entry_velocity(a, t.height(), t.distance(), g);
    ret.velocity = get_launch_velocity(a, t.distance(), vf, g);
    ret.angle = get_launch_angle(a, t.distance(), vf, g);
    return ret;
}

/* This calculation relies upon a different methodology.  The angle theta
 * at which a projectile needs to be launched in order to have a displacement
 * (x, y) when the velocity is v at the intial point is:
 * 
 *                             ______________________
 *               [    v^2 +/- / v^4 - g(gx^2 + 2yv^2)    ]
 * theta = arctan[ ------------------------------------- ]
 *               [                  gx                   ]
 * 
 */

trajectory calculate_trajectory_launchspeed(const target& t, double v, double g) {
    trajectory ret;
    double v_2 = v * v;
    double v_4 = v_2 * v_2;
    double x = t.distance();
    double x_2 = x * x;
    double y = t.height();
    double discrim = v_4 - (g * ((g * x_2) + (2 * y * v_2)));
    if (discrim < 0) {
        //unable to reach point
        ret.velocity = 0.0;
        ret.angle = 0.0;
        return ret;
    }
    discrim = std::sqrt(discrim);
    double theta = std::atan2(v_2 + discrim, g * x);
    if (theta < pi_over4) {
        theta = pi_over2 - theta;
    }
    ret.velocity = v;
    ret.angle = theta;
    return ret;    
}

trajectory projected_trajectory(target* t) {
    trajectory ret_trajectory = calculate_trajectory_entryangle(*t, DEFAULT_ENTRYANGLE_BACKBOARD);
    if (ret_trajectory.velocity > shooter::rps_to_ballspeed(75.0, ret_trajectory.angle)) {
        //unable to do - fallback
        ret_trajectory = calculate_trajectory_launchspeed(*t, DEFAULT_LAUNCHSPEED);
        if (ret_trajectory.velocity == 0.0) {
            //really unable to do
            return (trajectory){0,0};
        }
    }
    return ret_trajectory;
}
