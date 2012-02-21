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


void do_turret_rotation();
void do_bridge_arm();
void do_turret_winch();
void show_pot_voltage();
void do_launcher_wheel();
void do_rollers();

const int TURRET_ROTATION        = 1;
const int BRIDGE_ARM_DOWN        = 8;
const int BRIDGE_ARM_UP          = 9;
const int WINCH_75               = 3;
const int WINCH_UP               = 3;
const int WINCH_80               = 2;
const int WINCH_DOWN             = 2;
const int POT_VOLTAGE            = 10;
const int LAUNCHER_WHEEL         = 4;
const int ROLLERS_DOWN           = 6;
const int ROLLERS_UP             = 7;

launch_counter launch_wheel_counter(launcher_wheel);
two_jags launch_wheel_jags(left_launcher_jag, right_launcher_jag);
pid_controller launch_pid(0.005, 0.00, 0.002, &launch_wheel_counter, &launch_wheel_jags);
winch winch_obj(turret_winch_jag, launch_angle_pot, launch_angle_switch);

void gunner_override_controls() {
    do_turret_rotation();
    do_bridge_arm();
    do_turret_winch();
    show_pot_voltage();
    do_launcher_wheel();
    do_rollers();
}

void do_turret_rotation() {
    //Turret rotation controlled by gunner joystick during drive state only. Must press button 1
    if (gunner_joystick.GetRawButton(TURRET_ROTATION)) {
        printf("turret ROTATE\n");
        turret_rotation_jag.Set(-gunner_joystick.GetX());
    }
}

void do_bridge_arm() {
    if(gunner_joystick.GetRawButton(BRIDGE_ARM_UP)){//up
        if(!bridge_arm_switch.Get()){//limit switch not pressed
            printf("bridge arm UP\n");
            bridge_arm_spike.Set(Relay::kForward);
        }
    }
    else if(gunner_joystick.GetRawButton(BRIDGE_ARM_DOWN)){//down
        printf("bridge arm DOWN\n");
        bridge_arm_spike.Set(Relay::kReverse);
    }
    else {
        bridge_arm_spike.Set(Relay::kOff);
    }
}

float winch_z = 0;
bool angle_changed = false;
void do_turret_winch() {
/*    if(left_joystick.GetRawButton(WINCH_UP)) {
        printf("winch UP\n");
        turret_winch_jag.Set(-0.2);
    }
    else if(left_joystick.GetRawButton(WINCH_DOWN)) {
        printf("winch DOWN\n");
        turret_winch_jag.Set(0.2);
    }
    else {
        turret_winch_jag.Set(0.0);
    }*/
    float new_winch_z = (-left_joystick.GetZ()+1)*22.5+45;
    if(new_winch_z != winch_z) {
        winch_z = new_winch_z;
        printf("winch (desired) angle: %f\n", new_winch_z);
        angle_changed = true;
    }
    if(gunner_joystick.GetRawButton(WINCH_75)) {
        printf("winch (desired) angle: 75\n");
        winch_obj.disable();
        winch_obj.set_angle(deg2rad(75));
    }
    else if(gunner_joystick.GetRawButton(WINCH_80)) {
        if(angle_changed) {
            printf("winch TURN\n");
            winch_obj.disable();
            winch_obj.set_angle(deg2rad(winch_z));
            angle_changed = false;
        }
    }
}

void show_pot_voltage() {
    if(gunner_joystick.GetRawButton(POT_VOLTAGE)) {
        printf("pot voltage: %f\n", launch_angle_pot.GetVoltage());
    }
}

bool setpoint_set = false;
bool timer_started = false;
float setpoint = 0;
Timer timer;
float prev_z = 0;
void do_launcher_wheel() {
    float new_z = (-gunner_joystick.GetZ()+1)*30+20;
    if (new_z != prev_z) {
        prev_z = new_z;
        printf("launcher rps: %f\n", new_z);
    }
    if (gunner_joystick.GetRawButton(LAUNCHER_WHEEL) && !timer_started && !setpoint_set) {
        printf("launcher wheel SPIN (%f rps)\n", prev_z);
        timer.Start();
        timer_started = true;
        setpoint = prev_z;
        launch_wheel_jags.Set(0.75);
    }
    else if (timer_started && timer.HasPeriodPassed(0.25)) {
        timer.Stop();
        launch_pid.SetSetpoint(setpoint);
        launch_pid.Enable();
        setpoint_set = true;
    }
    else if (!gunner_joystick.GetRawButton(LAUNCHER_WHEEL)) {
        if(setpoint_set) {
            launch_pid.Disable();
            setpoint_set = false;
        }
        if(timer_started) {
            timer.Stop();
            timer_started = false;
        }
        launch_wheel_jags.Set(-0.15);
    }
    if (timer_started || setpoint_set) {
        printf("Launcher Wheel Speed: %f\n", 1/launcher_wheel.GetPeriod());
    }
}

void do_rollers() {
    if (gunner_joystick.GetRawButton(ROLLERS_UP)) {
        printf("rollers UP\n");
        rollers.set_direction(roller_t::UP);
    }
    else if (gunner_joystick.GetRawButton(ROLLERS_DOWN)) {
        printf("rollers DOWN\n");
        rollers.set_direction(roller_t::DOWN);
    }
    else {
        rollers.set_direction(roller_t::OFF);
    }
}
