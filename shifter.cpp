#include "shifter.h"
#include "update.h"

#include <Servo.h>

const float klowgear = 0.15;
const float kneutral = 0.5;
const float khighgear = 0.85;

shifter::shifter(Servo& a, Servo& b) {
    shift1 = &a;
    shift2 = &b;
    cur_gear = LOW;
    registry().register_func(update_helper, (void*)this);
}

shifter::~shifter() {
    registry().unregister_func(update_helper, (void*)this);
}

void shifter::set(GEAR g) {
    cur_gear = g;
}

void shifter::update() {
    const float * servoval;
    switch (cur_gear) {
        default:
        case LOW:
            servoval = &klowgear;
            break;
        case HIGH:
            servoval = &kneutral;
            break;
        case NEUTRAL:
            servoval = &khighgear;
            break;
    }
    shift1->Set(*servoval);
    shift2->Set(*servoval);
}

void shifter::update_helper(void * obj) {
    ((shifter*)obj)->update();
}
