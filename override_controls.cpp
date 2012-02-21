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
#include "ports.h"
#include "visionalg.h"
#include "launch_counter.h"
#include "pid_controller.h"
#include "two_jags.h"
#include "trajectory.h"
#include "winch.h"
#include "shooter.h"


void do_turret_rotation();
void do_bridge_arm();
void do_turret_winch();
void show_pot_voltage();
void do_launcher_wheel();
void do_rollers();

const int TURRET_ROTATION        = 2;
const int BRIDGE_ARM_DOWN        = 8;
const int BRIDGE_ARM_UP          = 9;
const int WINCH_ADJUST           = 3;
const int POT_VOLTAGE            = 10;
const int LAUNCHER_WHEEL         = 4;
const int ROLLERS_DOWN           = 6;
const int ROLLERS_UP             = 7;

void gunner_override_controls() {
    do_turret_rotation();
    do_bridge_arm();
    do_turret_winch();
    show_pot_voltage();
    do_launcher_wheel();
    do_rollers();
}

void do_turret_rotation() {
    if (gunner_joystick.GetRawButton(TURRET_ROTATION)) {
        shooter_turret.Susan().manual_control(gunner_joystick.GetX());
    }
    else {
        shooter_turret.Susan().manual_control(0.0);
    }
}

void do_bridge_arm() {
    if(gunner_joystick.GetRawButton(BRIDGE_ARM_UP)){//up
        bridge_arm.set_direction(bridge_arm_t::UP);
    }
    else if(gunner_joystick.GetRawButton(BRIDGE_ARM_DOWN)){//down
        bridge_arm.set_direction(bridge_arm_t::DOWN);
    }
    else {
        bridge_arm.set_direction(bridge_arm_t::NEUTRAL);
    }
}

void do_turret_winch() {
    static float winch_z = 0.0;
    static bool angle_changed = false;
    float new_winch_z = (-left_joystick.GetZ()+1)*22.5+45;
    if(new_winch_z != winch_z) {
        winch_z = new_winch_z;
        printf("winch (desired) angle: %f\n", new_winch_z);
        angle_changed = true;
    }
    if (gunner_joystick.GetRawButton(WINCH_ADJUST) && angle_changed) {
        shooter_turret.Winch().set_angle(deg2rad(winch_z));
        angle_changed = false;
    }
}

void show_pot_voltage() {
    if(gunner_joystick.GetRawButton(POT_VOLTAGE)) {
        printf("pot voltage: %f\n", launch_angle_pot.GetVoltage());
    }
}

void do_launcher_wheel() {
    static float prev_z = 0.0;
    static bool enabled = false;
    float new_z = (-gunner_joystick.GetZ()+1)*30+20;
    if (new_z != prev_z) {
        prev_z = new_z;
        printf("launcher rps: %f\n", new_z);
    }
    if (!enabled) {
        if (gunner_joystick.GetRawButton(LAUNCHER_WHEEL)) {
            enabled = true;
            shooter_turret.Shooter().set_freq(prev_z);
            shooter_turret.Shooter().enable();
        }
    }
    else if (!gunner_joystick.GetRawButton(LAUNCHER_WHEEL)) {
        enabled = false;
        shooter_turret.Shooter().disable();
    }
}

void do_rollers() {
    if (gunner_joystick.GetRawButton(ROLLERS_UP)) {
        rollers.set_direction(roller_t::UP);
    }
    else if (gunner_joystick.GetRawButton(ROLLERS_DOWN)) {
        rollers.set_direction(roller_t::DOWN);
    }
    else {
        rollers.set_direction(roller_t::OFF);
    }
}
