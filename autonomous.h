/* states/state_autonomous.h
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
 * autonomous code
 */

#ifndef STATE_AUTONOMOUS_H_INCLUDED
#define STATE_AUTONOMOUS_H_INCLUDED

#include "state_tracker.h"
#include "trajectory.h"

#define DRIVE_BACK_ENABLED
// #define DRIVE_BACK_DISABLED

// #define BALLS_FIXED
#define BALLS_INFINITE

const int DISTANCE_TO_BRIDGE = -100; // inches
const int DISTANCE_FORWARD = 32;
const double SHOOT_FREQ = 62.7; // 70.78;
const double LAUNCH_ANGLE_DEG = 74.4; // 66.0;
const double LAUNCH_ANGLE_RAD = deg2rad(LAUNCH_ANGLE_DEG);
const double AUTNOM_NUM_BALLS = 2;
const double BRIDGEARM_WAIT = 1.5;

enum AUTONOMOUS_SUBSTATE {
    AUTNOM_STATE_SETUP,
    AUTNOM_STATE_DRIVE,
    AUTNOM_STATE_LAUNCH,
    AUTNOM_STATE_BRIDGEARM
};

extern state_tracker autonomous_substate;

void autonomous_init();
void do_autonomous();

#endif
