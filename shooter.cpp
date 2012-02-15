#include "shooter.h"

#include <PIDController.h>
#include <Counter.h>
#if DEBUG_612
#include <SmartDashboard/SendablePIDController.h>
#include <SmartDashboard/SmartDashboard.h>
#endif

//PID Gains
const double PID_P = 0.0;
const double PID_I = 0.0;
const double PID_D = 0.0;
const float tolerance = 0.10;

shooter::shooter(Counter& c, Jaguar& a, Jaguar& b) {
    jags = new two_jags(a, b);
    speed = new launch_counter(c);
        //allocate PID Control
#if DEBUG_612
    control = new SendablePIDController(PID_P, PID_I, PID_D, speed, jags);
    key = "LAUNCH_PID"; //TODO: If necessary, set this to be unique
    SmartDashboard::GetInstance()->PutData(key, (SendablePIDController*)control);
    //NOTE: THE ABOVE LINE IS DANGEROUS!!!!!!!!!!
#else
    control = new PIDController(PID_P, PID_I, PID_D, speed, jags);
#endif
}

shooter::~shooter() {
    delete speed;
    delete control;
    delete jags;
}

void shooter::set_speed(float s) {
    control->SetSetpoint(s);
}

void shooter::enable() {
    control->Enable();
}

void shooter::disable() {
    control->Disable();
}


