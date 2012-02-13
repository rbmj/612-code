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

#include "612.h"
#include "main.h"
#include "ports.h"
#include "update.h"
#include "vision/vision_processing.h"
#include "state_tracker.h"
#include "visionalg.h"

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
    init_camera();
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
    if(global_state.get_state() == STATE_DRIVING) {
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
            right_servo_shift.Set(0.7);
            left_servo_shift.Set(0.7);
            // set servo to high gear
        }
        else if (left_joystick.GetRawButton(10)) {
            right_servo_shift.Set(0.3);
            left_servo_shift.Set(0.3);
            //Sets servo to low gear
        }
    }
    else if(global_state.get_state() == STATE_AIMING) {
        // disable motor safety check to stop wasting netconsole space
        drive.SetSafetyEnabled(false);
        ColorImage* camera_image = vision_processing::get_image();
        vector<double> target_degrees = vision_processing::get_degrees_from_image(camera_image);
        vector<double> target_distances = vision_processing::get_distance_from_image(camera_image);
//        printf("Angle (degrees) of camera: %f", target_degrees[vision_processing::determine_aim_target_from_image(vision_processing::get_image())]);
        if(target_degrees.size() >= 1) {
            printf("Angle (degrees) of camera: %f\n", target_degrees[0]);
        }
        else {
            printf("No target detected\n");
        }
        if(target_distances.size() >= 1) {
            printf("Distance of target:       %f\n", target_distances[0]);
        }
        if(!left_joystick.GetRawButton(3)) {
            global_state.set_state(STATE_DRIVING);
            drive.SetSafetyEnabled(true);
        }
    }
    //stuff goes under
    if (left_joystick.GetRawButton(3)) { 
		left_launcher_jag.Set(0.1);
		right_launcher_jag.Set(0.1);
	}
    //stuff goes over
    if(global_state.get_state() != STATE_AIMING) {
        Wait(0.005); //let the CPU rest a little - 5 ms isn't too long
    }
}

void robot_class::update_sensors() {
    //run functions in update registry
    registry().update();
}

//the following macro tells the library that we want to generate code
//for our class robot_class
START_ROBOT_CLASS(robot_class);
