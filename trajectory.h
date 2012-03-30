/* trajectory.h
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
 * Provide algorithms to calculate the trajectory
 */
 
#ifndef TRAJECTORY_H_INC
#define TRAJECTORY_H_INC

#include "vision_alt.h"

struct trajectory {
    double angle; //radians
    double velocity; //fps
};

const double DEFAULT_ENTRYANGLE = 1.04719755; //60 degrees
const double DEFAULT_ENTRYANGLE_BACKBOARD = 0.261799388; //15 degrees
const double DEFAULT_LAUNCHSPEED = 25.0;
const double ACC_G = 32.17;

trajectory calculate_trajectory_entryangle(const target&, double, double = ACC_G);
trajectory calculate_trajectory_launchspeed(const target&, double, double = ACC_G);

trajectory inline calculate_trajectory(const target& t) {
#ifdef TRAJECTORY_LAUNCHSPEED
    return calculate_trajectory_launchspeed(t, DEFAULT_LAUNCHSPEED);
#elif defined TRAJECTORY_ENTRYANGLE
    return calculate_trajectory_entryangle(t, DEFAULT_ENTRYANGLE);
#else
#error Invalid Trajectory Specification in 612.h
#endif
}

trajectory projected_trajectory(target*);

const double pi = 3.14159265358979; // max precision :(
const double pi_over180 = pi / 180;
const double pi_180over = 180 / pi;
const double pi_over4 = pi / 4;
const double pi_over2 = pi / 2;
 
//stuff to calculate trajectory goes here!
double get_entry_velocity(double, double, double, double = ACC_G);
double get_launch_velocity(double, double, double, double = ACC_G);
double get_launch_angle(double, double, double, double = ACC_G);

double inline deg2rad(double theta) {
    return theta * pi_over180;
}

double inline rad2deg(double theta) {
    return theta * pi_180over;
}

#endif
