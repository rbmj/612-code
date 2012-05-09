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

#include "../612.h"
#include "../vision/vision_processing.h"
#include "../ports.h"
#include "../visionalg.h"
#include "../state_tracker.h"

const int ARCADE_DRIVE = 1;
const int SHIFT_HIGH_L = 6;
const int SHIFT_LOW_L  = 7;
const int SHIFT_HIGH_R = 11;
const int SHIFT_LOW_R  = 10;
const int SHOOT        = 5;

void do_driving();
void do_shifting();
void change_state();

void state_driving() {
    do_driving();
    do_shifting();
    change_state();
}

void do_driving() {
    if (left_joystick.GetRawButton(ARCADE_DRIVE)) {
        //arcade drive
        drive.ArcadeDrive(left_joystick); //arcade drive on left joystick
    }
    else {
        //tank drive
        // Drive changed To Shan's orders.
        void Drive (right_joystick.GetY,left_joystick.GetX);
        float left = left_joystick.GetX();
        float right = right_joystick.GetY();
        std::printf("Tank Drive: %f, %f\n", left, right);
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
    /*
    if(gunner_joystick.GetRawButton(SHOOT)) {
        global_state.set_state(STATE_SHOOTING);
    }
    */
}
