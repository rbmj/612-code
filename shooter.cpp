#include "shooter.h"
#include "pid_controller.h"
#include "update.h"

#include <PIDController.h>
#include <Counter.h>
#include <Timer.h>

#include <cstdio>

//PID Gains
const double WHEEL_P = 0.005;
const double WHEEL_I = 0.0;
const double WHEEL_D = 0.002;
const float WHEEL_TOLERANCE = 4.0; //percent

shooter::shooter(Counter& c, Jaguar& a, Jaguar& b) {
    jags = new two_jags(a, b);
    speed = new launch_counter(c);
    //allocate PID Control
    control = new pid_controller(WHEEL_P, WHEEL_I, WHEEL_D, speed, jags);
    control->SetTolerance(WHEEL_TOLERANCE);
    control->SetInputRange(0.0, 75.0);
    control->SetOutputRange(-0.25, 0.25);
    setpoint = 0.0;
    //shooter starts disabled by default.  Must enable to spin up wheels
    disable();
    
    registry().register_func(shooter::update_help, (void*)this);
}

shooter::~shooter() {
    disable();
    delete speed;
    delete control;
    delete jags;
    
    registry().unregister_func(shooter::update_help, (void*)this);
}

void shooter::set_speed(double s) {
    setpoint = ballspeed_to_rps(s);
}

void shooter::set_freq(double s) {
    setpoint = s;
}

void shooter::enable() {
    enabled = true;
}

void shooter::disable() {
    enabled = false;
    setpoint_set = false;
    timer_started = false;
    arrived_at_speed = false;
    launch_time.Stop();
    control->Disable();
}

double shooter::ballspeed_to_rps(double speed) {
    //TODO: Implement.  Use high speed camera?
    return speed;
}

void shooter::update_help(void * obj) {
    ((shooter*)obj)->update();
}

void shooter::update() {
    if (!enabled) {
        jags->Set(-0.175);
        return;
    }
    if (!timer_started) {
        launch_time.Start();
        launch_time.Reset();
        timer_started = true;
    }
    if (timer_started && !setpoint_set) {
        if (launch_time.HasPeriodPassed(0.25)) {
            control->SetSetpoint(setpoint);
            control->Enable();
            setpoint_set = true;
        }
        else if (setpoint > 50) {
            jags->Set(1.0);
        }
        else {
            jags->Set(0.7);
        }
    }
    if (setpoint_set) {
        //PID control
#if DEBUG_612
        bool at_target = control->OnTarget();
        if (!arrived_at_speed && at_target) {
            arrived_at_speed = true;
            std::printf("shooter: arrived at speed %f\n", setpoint);
        }
        if (arrived_at_speed && !at_target) {
            arrived_at_speed = false;
        }
#endif
    }
}
