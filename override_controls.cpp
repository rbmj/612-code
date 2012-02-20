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
#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

void do_turret_rotation();
void do_bridge_arm();
void do_turret_winch();
void show_pot_voltage();
void do_launcher_wheel();
void do_rollers();

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
    if (gunner_joystick.GetRawButton(1)) {
        printf("turret ROTATE\n");
        turret_rotation_jag.Set(-gunner_joystick.GetX());
    }
}

void do_bridge_arm() {
    if(gunner_joystick.GetRawButton(2)){//up
        if(bridge_arm_switch.Get()!=1){//limit switch not pressed
            printf("bridge DOWN\n");
            bridge_arm_spike.Set(Relay::kForward);
        }
    }
    else if(gunner_joystick.GetRawButton(3)){//down
        printf("bridge UP\n");
        bridge_arm_spike.Set(Relay::kReverse);
    }
    else {
        bridge_arm_spike.Set(Relay::kOff);
    }
}

void do_turret_winch() {
    if(gunner_joystick.GetRawButton(7)) {
        printf("winch UP\n");
        turret_winch_jag.Set(0.2);
    }
    else if(gunner_joystick.GetRawButton(6)) {
        printf("winch DOWN\n");
        turret_winch_jag.Set(-0.2);
    }
    else {
        turret_winch_jag.Set(0.0);
    }
}

void show_pot_voltage() {
    if(gunner_joystick.GetRawButton(8)) {
        printf("pot voltage: %f\n", launch_angle_pot.GetVoltage());
    }
}

void do_launcher_wheel() {
    if (gunner_joystick.GetRawButton(4)) {
        printf("launcher wheel SPIN\n");
        left_launcher_jag.Set(1.0);
        right_launcher_jag.Set(1.0);
        printf("Launcher Wheel Speed: %f\n", 1/launcher_wheel.GetPeriod());
    }
    else {
        left_launcher_jag.Set(-0.05);
        right_launcher_jag.Set(-0.05);
    }
}

void do_rollers() {
    if (gunner_joystick.GetRawButton(11)) {
        printf("rollers UP\n");
        rollers.set_direction(roller_t::UP);
    }
    else if (gunner_joystick.GetRawButton(10)) {
        printf("rollers DOWN\n");
        rollers.set_direction(roller_t::DOWN);
    }
    else {
        rollers.set_direction(roller_t::OFF);
    }
}
