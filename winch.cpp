#include <cmath>
#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>
#include "winch.h"
//#include "winch_motor.h"
#include "update.h"

const float WINCH_TOLERANCE = 0.025;
const float WINCH_SPEED = 0.2;

void winch_update_helper(void*);

winch::winch(Jaguar& j, AnalogChannel& c, DigitalInput& l) {
    jag = &j;
    pot = &c;
    limit = &l;
    //winch starts enabled by default, as winch should always be enabled and
    //updating.
    enabled = true;
    desired_pot_voltage = pot->GetVoltage();
    registry().register_func(winch_update_helper, (void*)this);
}

winch::~winch() {
    //
}

double winch::launch_angle_to_voltage(double theta) {
    //takes an angle in radians and converts it to a voltage.
    //note that the angle starts at pi/2 and decreases to pi/4
    //TODO: implement
    return (-0.9493487159 * theta) + 3.9019452071;
}

void winch::enable() {
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

void winch::update() {
    if(!enabled) {
        jag->Set(0.0);
        return;
    }
/*    if(limit->Get()) {
        disable();
    }*/
    float current_pot_voltage = pot->GetVoltage();
    float pot_voltage_diff = desired_pot_voltage - current_pot_voltage;
    if(abs((int)(pot_voltage_diff * 100)) < (int)(WINCH_TOLERANCE * 100)) {
        printf("arrived at angle\n");
        disable();
        return;
    }
    if(pot_voltage_diff > 0) {
        jag->Set(WINCH_SPEED);
    }
    else {
        jag->Set(-WINCH_SPEED);
    }
}

void winch_update_helper(void* thisPtr) {
    ((winch*)thisPtr)->update();
}
