#include <cmath>
#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>
#include "winch.h"
#include "update.h"
#include "trajectory.h"

const float WINCH_TOLERANCE = 0.075;
const float WINCH_SPEED = 0.35;

void winch_update_helper(void*);

winch::winch(Jaguar& j, AnalogChannel& c, DigitalInput& l) {
    jag = &j;
    pot = &c;
    limit = &l;
    //winch starts enabled by default, as winch should always be enabled and
    //updating.
    desired_pot_voltage = pot->GetVoltage();
    enabled = true;
    registry().register_func(winch_update_helper, (void*)this);
}

winch::~winch() {
    //
}

float winch::launch_angle_to_voltage(float theta) {
    //takes an angle in radians and converts it to a voltage.
    //note that the angle starts at pi/2 and decreases to pi/4
    return (-1.0207443959 * theta) + 4.1827946896;
}

float winch::voltage_to_launch_angle(float voltage) {
    //the inverse of the above
    float launch_angle = (-0.9129997242 * voltage) + 3.9002999384;
    return launch_angle;
}

void winch::enable() {
    desired_pot_voltage = pot->GetVoltage();
    enabled = true;
}

void winch::disable() {
    enabled = false;
}

bool winch::is_enabled() const {
    return enabled;
}

void winch::set_angle(float theta) { // angle of launch in radians, not angle of winch
    if(theta < 0.77 || theta > 1.58) {
        return;
    }
    desired_pot_voltage = launch_angle_to_voltage(theta);
    desired_angle = theta;
}

float winch::get_set_angle() const {
    return desired_angle;
}

float winch::get_cur_angle() const {
    return voltage_to_launch_angle(pot->GetVoltage());
}

void winch::manual_control(direction_t direction) {
    if(direction == UP) {
        jag->Set(-WINCH_SPEED);
    }
    else if(direction == DOWN) {
//        if (limit->Get()) {
            jag->Set(WINCH_SPEED);
/*        }
        else {
            jag->Set(0.0);
        }*/
    }
    else {
        jag->Set(0.0);
    }
}

void winch::update() {
    if(!enabled) {
        jag->Set(0.0);
        return;
    }
    float current_pot_voltage = pot->GetVoltage();
    float pot_voltage_diff = desired_pot_voltage - current_pot_voltage;
    if(abs((int)(pot_voltage_diff * 100)) < (int)(WINCH_TOLERANCE * 100)) {
        jag->Set(0.0);
        return;
    }
    if(pot_voltage_diff > 0) {
        if (limit->Get()) {
            jag->Set(WINCH_SPEED);
        }
        else {
            jag->Set(0.0);
        }
    }
    else {
        jag->Set(-WINCH_SPEED);
    }
}

void winch_update_helper(void* thisPtr) {
    ((winch*)thisPtr)->update();
}
