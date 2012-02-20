#include "winch.h"

#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>

const double WINCH_P = 0.0;
const double WINCH_I = 0.0;
const double WINCH_D = 0.0;
const float WINCH_TOLERANCE = 0.10;
const int POT_VOLTAGE_B = 0; // look at equation

winch::winch(Jaguar& j, AnalogChannel& c, DigitalInput& l) {
    jag = &j;
    pot = &c;
    limit = &l;
    pid = new pid_controller(WINCH_P, WINCH_I, WINCH_D, pot, jag);
    pid->SetTolerance(WINCH_TOLERANCE);
}

winch::~winch() {
    delete pid;
}

double winch::angle_to_voltage(double theta) {
    //takes an angle in radians and converts it to a voltage.
    //note that the angle starts at pi/2 and decreases to pi/4
    //TODO: implement
    int pot_voltage_m = 0; // look at equation
    return (pot_voltage_m * theta) + POT_VOLTAGE_B;
}

double winch::launch_angle_to_winch_angle(double theta) {
    double new_theta = theta;
    // black magic
    return theta;
}

void winch::enable() {
    pid->Enable();
}

void winch::disable() {
    pid->Disable();
}

void winch::set_angle(double theta) { // angle of launch, not angle of winch
    theta = launch_angle_to_winch_angle(theta);
    pid->SetSetpoint(angle_to_voltage(theta));
}
