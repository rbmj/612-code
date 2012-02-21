#include <cmath>
#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>
#include "winch.h"
//#include "winch_motor.h"
#include "update.h"
#include "trajectory.h"

const float WINCH_TOLERANCE = 0.025;
const float WINCH_SPEED = 0.2;

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

double winch::launch_angle_to_voltage(double theta) {
    //takes an angle in radians and converts it to a voltage.
    //note that the angle starts at pi/2 and decreases to pi/4
    return (-0.9493487159 * theta) + 3.9019452071;
}

double winch::voltage_to_launch_angle(double voltage) {
    double radians = (-1.0466681558 * voltage) + 4.0902451845;
    return rad2deg(radians);
}

void winch::enable() {
    desired_pot_voltage = pot->GetVoltage();
    enabled = true;
}

void winch::disable() {
    enabled = false;
}

bool winch::is_enabled() {
    return enabled;
}

void winch::set_angle(double theta) { // angle of launch in radians, not angle of winch
    printf("theta: %f\n", theta);
    if(theta < 0.77 || theta > 1.58) {
        return;
    }
    desired_pot_voltage = launch_angle_to_voltage(theta);
    printf("pot voltage: %f\n", desired_pot_voltage);
//    pid->SetSetpoint(pot_voltage);
}

void winch::manual_control(direction_t direction) {
    if(direction == UP) {
        jag->Set(-WINCH_SPEED);
    }
    else if(direction == DOWN) {
        if (limit->Get()) {
            jag->Set(WINCH_SPEED);
        }
        else {
            jag->Set(0.0);
        }
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
