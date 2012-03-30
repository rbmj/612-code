/* states/state_driving.cpp
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
 * driving state
 */
 
#include <cmath>
#include "state_driving.h"
#include "../612.h"
#include "../vision/vision_processing.h"
#include "../ports.h"
#include "../visionalg.h"
#include "../state_tracker.h"
#include "../encoder.h"

const int ARCADE_DRIVE = 1;
const int SHIFT_HIGH_L = 6;
const int SHIFT_LOW_L  = 7;
const int SHIFT_HIGH_R = 11;
const int SHIFT_LOW_R  = 10;
const int DRIVE_BACK   = 8; // right joy
const int DRIVE_BACK_DOUBLE = 9; // right joy
const double DRIVE_BACK_DISTANCE = -14; // inches
const double JOY_TOLERANCE = 0.2;

void do_driving();
void do_shifting();
static void change_state();

void state_driving() {
    do_driving();
    do_shifting();
    change_state();
}

void do_driving() {
    if (right_joystick.GetRawButton(DRIVE_BACK)) {
        EncoderWheels::GetInstance().SetDistance(DRIVE_BACK_DISTANCE);
    } else if (right_joystick.GetRawButton(DRIVE_BACK_DOUBLE)) {
        EncoderWheels::GetInstance().SetDistance(DRIVE_BACK_DISTANCE*2);
    } else if (left_joystick.GetRawButton(ARCADE_DRIVE)) {
        EncoderWheels::GetInstance().Disable();
        //arcade drive
        drive.ArcadeDrive(left_joystick); //arcade drive on left joystick
    }
    else {
        //tank drive
        float left = left_joystick.GetY();
        float right = right_joystick.GetY();
        if(EncoderWheels::GetInstance().IsEnabled()) {
            if(std::fabs(left) > JOY_TOLERANCE || std::fabs(right) > JOY_TOLERANCE) {
                EncoderWheels::GetInstance().Disable();
            //explicitly state drive power is based on Y axis of that side joy
                drive.TankDrive(left, right);
            }
        }
        else {
            drive.TankDrive(left, right);
        }
    }
    static unsigned iterate = 0;
    iterate++;
    if(iterate%4==0) {
        std::printf("current distance (encoders): %f\n", EncoderWheels::GetInstance().GetCurDistance(EncoderWheels::DISTANCE_LEFT));
    }
}

void do_shifting() {
    if (right_joystick.GetRawButton(SHIFT_HIGH_R) || left_joystick.GetRawButton(SHIFT_HIGH_L)) {
        // set servo to high gear
        servo_shifter.set(shifter::HIGH);
    }
    else if (left_joystick.GetRawButton(SHIFT_LOW_L) || right_joystick.GetRawButton(SHIFT_LOW_R)) {
        // set servo to low gear
        servo_shifter.set(shifter::LOW);
    }
}

void change_state() {
    if(gunner_joystick.GetRawButton(SHOOT)) {
        global_state.set_state(STATE_SHOOTING);
    }
}
