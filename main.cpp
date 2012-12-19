/* main.cpp
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
 * Implement robot_class to provide functionality for robot.
 */

#include <PWM.h>
#include <cmath>
#include "612.h"
#include "main.h"
#include "ports.h"
#include "update.h"
#include "state_tracker.h"
#include "shifter.h"
#include "launch_counter.h"
#include "pid_controller.h"
#include "two_jags.h"
#include "visionalg.h"
#include "override_controls.h"
#include "vision_alt.h"
#include "trajectory.h"
#include "autonomous.h"
#include "encoder.h"
#include "states/state_shooting.h"
#include "states/state_driving.h"

//constructor - initialize drive
robot_class::robot_class() {
    //do nothing
    GetWatchdog().SetEnabled(false); //we don't want Watchdog
}

void robot_class::RobotInit() {
    //Run-Time INIT
    //set necessary inversions
    drive.SetInvertedMotor(left_front_motor.type,  left_front_motor.reverse);
    drive.SetInvertedMotor(left_rear_motor.type,   left_rear_motor.reverse);
    drive.SetInvertedMotor(right_front_motor.type, right_front_motor.reverse);
    drive.SetInvertedMotor(right_rear_motor.type,  right_rear_motor.reverse);
    global_state.set_state(STATE_DRIVING);
    init_camera();
    launcher_wheel.Start();
    global_state.register_func(STATE_DRIVING, state_driving);
    global_state.register_func(STATE_SHOOTING, state_shooting);
    EncoderWheels::Init(left_drive, right_drive, left_front_motor.jag, right_front_motor.jag, left_rear_motor.jag, right_rear_motor.jag);
    EncoderWheels::GetInstance().Enable();
}

void robot_class::DisabledInit() {
    //disable shooter
    shooter_turret.Shooter().disable();
    EncoderWheels::GetInstance().Disable();
    rollers.set_direction(roller_t::OFF);
}

void robot_class::AutonomousInit() {
    autonomous_init();
    shooter_turret.Shooter().disable();
}

void robot_class::TeleopInit() {
    // enable safety because disabled in autonomous
    drive.SetSafetyEnabled(true);
    shooter_turret.Shooter().disable();
    //buttons b(gunner_joystick);
    //gunner_override_controls(b);
}

void robot_class::DisabledPeriodic() {
    //do nothing
}

void robot_class::AutonomousPeriodic() {
    update_sensors();
}

void robot_class::TeleopPeriodic() {
    update_sensors();
}

void robot_class::DisabledContinuous() {
    //do nothing
}

void robot_class::AutonomousContinuous() {
    do_autonomous();
//    Wait(0.05);
}

void robot_class::TeleopContinuous() {
    gunner_override_controls();
    global_state.run_state();
    if (global_state.get_state() != STATE_SHOOTING) {
        Wait(0.005); //let the CPU rest a little - 1 ms isn't too long
    }
}

void robot_class::update_sensors() {
    //run functions in update registry
    registry().update();
    //power on LEDs
    camera_led_digital.Set(1);
    //camera_led.SetRaw(255); //not using pwm
//    set_target();
}

void robot_class::set_target() {
    if (gunner_joystick.GetRawButton(3)) {
        //get target from vision processing
    }
}

//the following macro tells the library that we want to generate code
//for our class robot_class
START_ROBOT_CLASS(robot_class);
