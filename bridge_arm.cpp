#include "bridge_arm.h"
#include "update.h"
#include <Relay.h>
#include <DigitalInput.h>

bridge_arm_t::bridge_arm_t(Relay& r, DigitalInput& l) {
    spike = &r;
    limit = &l;
    dir = UP;
    registry().register_func(bridge_arm_t::update_help, (void*)this);
}

bridge_arm_t::~bridge_arm_t() {
    registry().unregister_func(bridge_arm_t::update_help, (void*)this);
}

void bridge_arm_t::update_help(void * obj) {
    ((bridge_arm_t*)obj)->update();
}

void bridge_arm_t::set_direction(bridge_arm_t::direction d) {
    if (d != UP || !limit->Get()) { //not going up OR limit not presed
        dir = d;
    }
}

bridge_arm_t::direction bridge_arm_t::get_direction() {
    return dir;
}

void bridge_arm_t::update() {
    Relay::Value v;
    switch (dir) {
    case UP:
        if (limit->Get()) { //pressed, turn off
            v = Relay::kOff;
            dir = NEUTRAL;
        }
        else {
            v = Relay::kForward;
        }
        break;
    case DOWN:
        v = Relay::kReverse;
        break;
    default:
        v = Relay::kOff;
        break;
    }
    spike->Set(v);
}
