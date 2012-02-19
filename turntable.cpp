#include "turntable.h"
#include "update.h"
#include "utility.h"

#include <Jaguar.h>
#include <DigitalInput.h>

//a proportional constant
const float TURNTABLE_MULTIPLIER = 1.0;
const int TURNTABLE_TOLERANCE = 5; //px

const bool RIGHT_POSITIVE = false;

turntable::turntable(Jaguar& j, DigitalInput& l, DigitalInput& m, DigitalInput& r) {
    jag = &j;
    left = &l;
    mid = &m;
    right = &r;
    
    registry().register_func(update_help, (void*)this);
    power = 0.0;
    enabled = false;
}

turntable::~turntable() {
    registry().unregister_func(update_help, (void*)this);
}

void turntable::update_help(void * obj) {
    ((turntable*)obj)->update();
}

void turntable::update() {
    if (enabled) {
        bool right_val = right->Get();
        bool left_val = left->Get();
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
            jag->Set(0.0);
        }
        else {
            if (centering) {
                //implement
            }
            else {
                jag->Set(power);
            }
        }
    }
}

void turntable::enable() {
    enabled = true;
}

void turntable::disable() {
    enabled = false;
}

void turntable::new_offset(int offset, int max) {
    power = coerce((offset / max) * TURNTABLE_MULTIPLIER, -1.0f, 1.0f);
    if (!RIGHT_POSITIVE) {
        power = -power;
    }
}

void turntable::center() {
    centering = true;
}
