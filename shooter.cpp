#include "shooter.h"
#include "pid_controller.h"

#include <PIDController.h>
#include <Counter.h>

//PID Gains
const double WHEEL_P = 0.0;
const double WHEEL_I = 0.0;
const double WHEEL_D = 0.0;
const float WHEEL_TOLERANCE = 0.10; //percent

shooter::shooter(Counter& c, Jaguar& a, Jaguar& b) {
    jags = new two_jags(a, b);
    speed = new launch_counter(c);
    //allocate PID Control
    control = new pid_controller(WHEEL_P, WHEEL_I, WHEEL_D, speed, jags);
    control->SetTolerance(WHEEL_TOLERANCE);
}

shooter::~shooter() {
    delete speed;
    delete control;
    delete jags;
}

void shooter::set_speed(double s) {
    control->SetSetpoint(ballspeed_to_rps(s));
}

void shooter::enable() {
    control->Enable();
}

void shooter::disable() {
    control->Disable();
}

double shooter::ballspeed_to_rps(double speed) {
    //TODO: Implement.  Use high speed camera?
    return speed;
}
