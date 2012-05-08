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

#include <cmath>
#include <Timer.h>
#include "autonomous.h"
#include "state_tracker.h"
#include "ports.h"
#include "encoder.h"
#include "vision/vision_processing.h"

#include <vector>

const double SHOT_WAIT = 0.25;
state_tracker autonomous_substate;

void autonomous_substate_setup();
void autonomous_substate_launch();
void autonomous_substate_drive();
void autonomous_substate_bridgearm();

void autonomous_init() {
    // disable safety: we don't drive using RobotDrive
    drive.SetSafetyEnabled(false);
    shooter_turret.Shooter().disable();
    autonomous_substate.set_state(AUTNOM_STATE_SETUP);
    autonomous_substate.register_func(AUTNOM_STATE_SETUP,autonomous_substate_setup);
    autonomous_substate.register_func(AUTNOM_STATE_LAUNCH,autonomous_substate_launch);
    autonomous_substate.register_func(AUTNOM_STATE_DRIVE,autonomous_substate_drive);
}

void do_autonomous() {
    autonomous_substate.run_state();
}

void autonomous_substate_setup() {
    shooter_turret.Winch().set_angle(LAUNCH_ANGLE_RAD);
    autonomous_substate.set_state(AUTNOM_STATE_DRIVE);
}

void autonomous_substate_launch() {
        if(shooter_turret.Shooter().at_setpoint()) {
            rollers.set_direction(roller_t::UP);
        }
}

void autonomous_substate_drive() {
    static bool distance_set = false;
    static Timer encoder_timer;
    if(!distance_set) {
        EncoderWheels::GetInstance().Disable();
        EncoderWheels::GetInstance().SetDistance(DISTANCE_FORWARD);
        distance_set = true;
        encoder_timer.Start();
    }
    else if(EncoderWheels::GetInstance().AtTarget() || encoder_timer.HasPeriodPassed(4.0)) {
        shooter_turret.Shooter().set_freq(SHOOT_FREQ);
        shooter_turret.Shooter().enable();
        autonomous_substate.set_state(AUTNOM_STATE_LAUNCH);
        EncoderWheels::GetInstance().Disable();
    }
}

void autonomous_substate_bridgearm() {
/*    static Timer arm_timer;
    static bool timer_started = false;
    if(timer_started) {
        if(arm_timer.HasPeriodPassed(BRIDGEARM_WAIT)) {
            bridge_arm.set_direction(bridge_arm_t::NEUTRAL);
            rollers.set_direction(roller_t::UP);
        }
        else {
            bridge_arm.set_direction(bridge_arm_t::DOWN);
        }
    }
    else {
        arm_timer.Start();
        timer_started = true;
    }*/
}
