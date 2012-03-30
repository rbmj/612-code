/* override_controls.cpp
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
 * Controls for overriding automatic shooting
 */
 
#include <cstdio>
#include <Timer.h>
#include "612.h"
#include "vision/vision_processing.h"
#include "override_controls.h"
#include "ports.h"
#include "visionalg.h"
#include "vision_alt.h"
#include "launch_counter.h"
#include "pid_controller.h"
#include "two_jags.h"
#include "trajectory.h"
#include "winch.h"
#include "shooter.h"
#include "buttons.h"
#include "vision_thread.h"

target KEY_TARGET = target::make_target(((98.0 + 11.0)/12) - robot_height, 12.0, 0);
target FENDER_TARGET = target::make_target(((98.0 + 11.0) / 12) - robot_height, 38.5/12, 0);

trajectory KEY_TRAJECTORY = calculate_trajectory_entryangle(KEY_TARGET, DEFAULT_ENTRYANGLE_BACKBOARD);
trajectory FENDER_TRAJECTORY = calculate_trajectory_entryangle(FENDER_TARGET, DEFAULT_ENTRYANGLE_BACKBOARD);

trajectory cur_trajectory;

void do_turret_rotation();
void do_bridge_arm();
void do_turret_winch(bool, bool, bool, bool);
void do_launcher_wheel(bool, bool, bool, bool, bool);
void do_rollers();

const int TURRET_ROTATION        = 1;
const int SHOOT_SPIN             = 2;
//const int ACQUIRE_TARGET         = 3;
const int SHOOT_KEY              = 4;
const int SHOOT_FENDER           = 5;
const int ROLLERS_UP             = 6;
const int ROLLERS_DOWN           = 7;
const int WINCH_OVERRIDE         = 8;
const int SHOOTER_OVERRIDE       = 9;
const int BRIDGE_ARM_DOWN        = 10;
const int BRIDGE_ARM_UP          = 11;

//don't ask me WHY this won't work with buttons& OR with GenericHID&
joysmooth& joy = gunner_joystick;

void gunner_override_controls() {
//    bool acquire = joy.GetRawButton(ACQUIRE_TARGET); // moved to states
    bool acquire = false;
    // legacy
    if (acquire) {
        target* t = ascertain_primary_target();
//        bool have_trajectory = false; // unused
        if (t) {
            //have target
            cur_trajectory = projected_trajectory(t);
        }
        else {
            cur_trajectory.velocity = 0.0;
            cur_trajectory.angle = 0.0;
        }
    }
    do_turret_rotation();
    do_bridge_arm();
    bool key = joy.GetRawButton(SHOOT_KEY);
    bool fender = joy.GetRawButton(SHOOT_FENDER);
    do_turret_winch(key, fender, joy.GetRawButton(WINCH_OVERRIDE), acquire);
    do_launcher_wheel(key, fender, joy.GetRawButton(SHOOTER_OVERRIDE), acquire, joy.GetRawButton(SHOOT_SPIN));
    do_rollers();
}

void do_turret_rotation() {
    if (joy.GetRawButton(TURRET_ROTATION)) {
        shooter_turret.Susan().manual_control(gunner_joystick.GetX());
    }
    else {
        shooter_turret.Susan().manual_control(0.0);
    }
}

void do_bridge_arm() {
    if(joy.GetRawButton(BRIDGE_ARM_UP)){//up
        bridge_arm_up();
    }
    else if(joy.GetRawButton(BRIDGE_ARM_DOWN)){//down
        bridge_arm_down();
    }
    else {
        bridge_arm_neutral();
    }
}

void bridge_arm_up() {
    bridge_arm.set_direction(bridge_arm_t::UP);
}

void bridge_arm_down() {
    bridge_arm.set_direction(bridge_arm_t::DOWN);
}

void bridge_arm_neutral() {
    bridge_arm.set_direction(bridge_arm_t::NEUTRAL);
}

void do_turret_winch(bool key, bool fender, bool override, bool acquire) {
    static float winch_z = 0.0;
/*
    if(gunner_joystick.GetRawButton(WINCH_UP)) {
        shooter_turret.Winch().disable();
        shooter_turret.Winch().manual_control(winch::UP);
    }
    else if(gunner_joystick.GetRawButton(WINCH_DOWN)) {
        shooter_turret.Winch().disable();
        shooter_turret.Winch().manual_control(winch::DOWN);
    }
    else if(!shooter_turret.Winch().is_enabled()) {
        shooter_turret.Winch().manual_control(winch::OFF);
        shooter_turret.Winch().enable();
    }
*/
    float new_winch_z = winch_z;
    if (key) {
        // hard coding
        //TODO: Remove
        new_winch_z = deg2rad(74.4); // 61.0
        //new_winch_z = KEY_TRAJECTORY.angle;
    }
    else if (fender) {
        // hard coding
        //TODO: Remove
        new_winch_z = deg2rad(86.3);
        //new_winch_z = FENDER_TRAJECTORY.angle;
    }
    else if (override) {
        new_winch_z = deg2rad((-gunner_joystick.GetZ()+1)*22.5+45);
    }
    else if (acquire) {
        if (cur_trajectory.velocity != 0) {
            new_winch_z = cur_trajectory.angle;
        }
    }
    if(new_winch_z != winch_z) {
        winch_z = new_winch_z;
//        std::printf("
        shooter_turret.Winch().set_angle(winch_z);
    }
/*    if(gunner_joystick.GetRawButton(8)) {
        shooter_turret.Winch().manual_control(winch::UP);
    }
    else if(gunner_joystick.GetRawButton(9)) {
        shooter_turret.Winch().manual_control(winch::DOWN);
    }
    else {
        shooter_turret.Winch().manual_control(winch::OFF);
    }*/
}

void do_launcher_wheel(bool key, bool fender, bool override, bool acquire, bool spin) {
    static float shoot_freq = 0.0;
    static bool enabled = false;
    float new_shoot_freq = shoot_freq;
    if (key) {
        /* hard coding */
        //TODO: Remove
        new_shoot_freq = 62.7; // 75.0
        //new_shoot_freq = shooter::ballspeed_to_rps(KEY_TRAJECTORY.velocity, KEY_TRAJECTORY.angle);
    }
    else if (fender) {
        /* hard coding */
        //TODO: Remove
        new_shoot_freq = 55.0;
        //new_shoot_freq = shooter::ballspeed_to_rps(FENDER_TRAJECTORY.velocity, FENDER_TRAJECTORY.angle);
    }
    else if (override) {
        new_shoot_freq = (-gunner_joystick.GetZ()+1)*15+45;
    }
    else if (acquire) {
        if (cur_trajectory.velocity != 0.0) {
            new_shoot_freq = shooter::ballspeed_to_rps(cur_trajectory.velocity, cur_trajectory.angle);
        }
    }
    if (new_shoot_freq != shoot_freq) {
        shoot_freq = new_shoot_freq;
        shooter_turret.Shooter().set_freq(shoot_freq);
    }
    if (!enabled) {
        if (spin) {
            enabled = true;
            shooter_turret.Shooter().enable();
        }
    }
    else if (!spin) {
        enabled = false;
        shooter_turret.Shooter().disable();
    }
}

void do_rollers() {
    if (joy.GetRawButton(ROLLERS_UP)) {
        rollers_up();
    }
    else if (joy.GetRawButton(ROLLERS_DOWN)) {
        rollers_down();
    }
    else {
        rollers_off();
    }
}

void rollers_up() {
    rollers.set_direction(roller_t::UP);
}

void rollers_down() {
    rollers.set_direction(roller_t::DOWN);
}

void rollers_off() {
    rollers.set_direction(roller_t::OFF);
}

void shoot_key() {
    do_launcher_wheel(true, false, false, false, false);
    do_turret_winch(true, false, false, false);
}

void shoot_fender() {
    do_launcher_wheel(false, true, false, false, false);
    do_turret_winch(false, true, false, false);
}

void shoot_spin() {
    do_launcher_wheel(false, false, false, false, true);
}

void turret_acquire() {
    do_launcher_wheel(false, false, false, true, false);
    do_turret_winch(false, false, false, true);
}
