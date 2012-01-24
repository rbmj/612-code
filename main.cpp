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

#include "main.h"
#include "ports.h"
#include "update.h"

//constructor - initialize drive
robot_class::robot_class() {
    GetWatchdog().SetEnabled(false); //we don't want Watchdog
    //now set the necessary inversions
    drive.SetInvertedMotor(left_front_motor.type,  left_front_motor.reverse);
    drive.SetInvertedMotor(left_rear_motor.type,   left_rear_motor.reverse);
    drive.SetInvertedMotor(right_front_motor.type, right_front_motor.reverse);
    drive.SetInvertedMotor(right_rear_motor.type,  right_rear_motor.reverse);
}

void robot_class::RobotInit() {
    //Run-Time INIT
    //do nothing
}

void robot_class::DisabledInit() {
    //do nothing
}

void robot_class::AutonomousInit() {
    //do nothing
}

void robot_class::TeleopInit() {
    //do nothing
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
    //do nothing
}

void robot_class::TeleopContinuous() {
    //actually do something!! :D
    if (left_joystick.GetRawButton(1)) {
        //arcade drive
        drive.ArcadeDrive(left_joystick); //arcade drive on left joystick
    }
    else {
        //tank drive
        float left = left_joystick.GetY();
        float right = right_joystick.GetY();
        //explicitly state drive power is based on Y axis of that side joy
        drive.TankDrive(left, right);
    }
    if (left_joystick.GetRawButton(11)) {
        servo :: set(0.7)
        // set servo to high gear
    }
    else if (left_joystick.GetRawbutton(10)) {
        servo :: set(0.5)
        //Sets servo to low gear
    }
    Wait(0.005); //let the CPU rest a little - 5 ms isn't too long
}

void robot_class::update_sensors() {
    //run functions in update registry
    registry.update();
}

//the following macro tells the library that we want to generate code
//for our class robot_class
START_ROBOT_CLASS(robot_class);
