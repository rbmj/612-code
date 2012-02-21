#include "turntable.h"
#include "update.h"
#include "utility.h"

#include <Jaguar.h>
#include <DigitalInput.h>

//a proportional constant
const float TURNTABLE_MULTIPLIER = 1.0;
const int TURNTABLE_TOLERANCE = 5; //px
const float TURNTABLE_DEFPOWER = 0.3;

const bool RIGHT_POSITIVE = false;

turntable::turntable(Jaguar& j, DigitalInput& l, DigitalInput& m, DigitalInput& r) {
    jag = &j;
    left = &l;
    mid = &m;
    right = &r;
    
    registry().register_func(update_help, (void*)this);
    power = 0.0;
    //enable by default
    enabled = true;
    centering = false;
    pos = UNSURE;
}

turntable::~turntable() {
    registry().unregister_func(update_help, (void*)this);
}

void turntable::update_help(void * obj) {
    ((turntable*)obj)->update();
}

void turntable::update() {
    bool right_val = !right->Get();
    bool left_val = !left->Get();
    bool center_val = !mid->Get();
    //at a limit
    if (right_val) {
        pos = RIGHT;
    }
    else if (center_val) {
        pos = CENTER;
    }
    else if (left_val) {
        pos = LEFT;
    }
    else if (pos == CENTER) {
        //passing over center, center limit no longer pressed.
        if ((RIGHT_POSITIVE && power > 0) || (!RIGHT_POSITIVE && power < 0)) {
            pos = RIGHT;
        }
        else if (power == 0) {
            //we got pushed.  Not sure anymore.
            pos = UNSURE;
        }
        else {
            pos = LEFT;
        }
    }
    //move turntable
    if (enabled) {
        if (
            (RIGHT_POSITIVE && (
                (power > 0 && right_val) || (power < 0 && left_val)
            ))
            ||
            (!RIGHT_POSITIVE && (
                (power < 0 && right_val) || (power > 0 && left_val)
            ))
        ) {
            //at a mechanical stop and trying to head into the mechanical stop
            power = 0.0;
        }
        else if (centering) {
            if (pos == UNSURE) {
                //just go in any direction.  When we hit a limit then we'll know
                //can't be at mechanical stop as limit switch never pressed.
                power = TURNTABLE_DEFPOWER;
            }
            else if (pos == LEFT) {
                //move right
                if (RIGHT_POSITIVE) {
                    power = TURNTABLE_DEFPOWER;
                }
                else {
                    power = -TURNTABLE_DEFPOWER;
                }
            }
            else if (pos == RIGHT) {
                //move left
                if (RIGHT_POSITIVE) {
                    power = -TURNTABLE_DEFPOWER;
                }
                else {
                    power = TURNTABLE_DEFPOWER;
                }
            }
            else {
                //we're already centered
                power = 0.0;
            }
        }
    }
    else {
        //disabled
        power = 0.0;
    }
    jag->Set(power);
}

void turntable::enable() {
    enabled = true;
}

void turntable::disable() {
    enabled = false;
}

void turntable::new_offset(int offset, int max) {
    centering = false;
    power = coerce((offset / max) * TURNTABLE_MULTIPLIER, -1.0f, 1.0f);
    if (!RIGHT_POSITIVE) {
        power = -power;
    }
}

void turntable::center() {
    centering = true;
}

void turntable::manual_control(float val) {
    if (!RIGHT_POSITIVE) {
        val = -val;
    }
    centering = false;
    power = coerce(val, -0.3f, 0.3f);
}
