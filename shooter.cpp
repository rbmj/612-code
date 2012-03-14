#include "shooter.h"
#include "pid_controller.h"
#include "update.h"

#include <PIDController.h>
#include <Counter.h>
#include <Timer.h>

#include <cstdio>
#include <cmath>

//PID Gains
const double WHEEL_P = 0.005;
const double WHEEL_I = 0.0;
const double WHEEL_D = 0.002;
const float WHEEL_TOLERANCE = 2.0; //percent
const float NUM_SHOT_TOLERANCE = 10.0;
const int MAX_RPS = 75;

float find_max_speed(float);

shooter::shooter(Counter& c, Jaguar& a, Jaguar& b) {
    jags = new two_jags(a, b);
    speed = new launch_counter(c);
    //allocate PID Control
    control = new pid_controller(WHEEL_P, WHEEL_I, WHEEL_D, speed, jags);
    control->SetTolerance(WHEEL_TOLERANCE);
    control->SetInputRange(0.0, 75.0);
    control->SetOutputRange(-0.25, 0.25);
    setpoint = 0.0;
    num_shot = 0;
    last_setpoint = 0.0;
    reached_target = false;
    ball_incremented = false;
    //shooter starts disabled by default.  Must enable to spin up wheels
    disable();
    
    registry().register_func(shooter::update_help, (void*)this);
}
unsigned int shooter::get_num_shot() {
	return num_shot;
}
void shooter::reset_num_shot() {
	num_shot = 0;
}
void shooter::update_num_shot () {
	if(std::fabs(speed->get_frequency()-setpoint)<NUM_SHOT_TOLERANCE && control->IsEnabled())
	{
		last_setpoint = control->GetSetpoint();
		reached_target = true;
		ball_incremented = false;
	}
	else if(control->IsEnabled())
	{
		if(last_setpoint != control->GetSetpoint())
		{
			reached_target = false;
		}
		else if(!ball_incremented)
		{
			num_shot++;
			ball_incremented = true;
		}
	}
}
shooter::~shooter() {
    disable();
    delete speed;
    delete control;
    delete jags;
    
    registry().unregister_func(shooter::update_help, (void*)this);
}

void shooter::set_speed(double s, double a) {
    setpoint = ballspeed_to_rps(s, a);
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

bool shooter::at_setpoint() {
    return control->OnTarget();
}

double shooter::ballspeed_to_rps(double speed,double Angle) {
    float Max = find_max_speed(Angle);
	float AmountNeeded = speed/Max;
	return (AmountNeeded*MAX_RPS);
}

double shooter::rps_to_ballspeed(double rps,double Angle) {
    float Max=find_max_speed(Angle);
    float AmountUsed = rps/MAX_RPS;
    return (AmountUsed*Max);
}

float find_max_speed(float Angle) {
	return ((46.2215843991*Angle) - 24.8498231509);
}

void shooter::update_help(void * obj) {
    ((shooter*)obj)->update();
}

void shooter::update() {
	update_num_shot();
    if (!enabled) {
        //backspin to keep balls in chute
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
    }
}

double shooter::get_cur_speed(double a) const {
    return rps_to_ballspeed(speed->get_frequency(), a);
}

double shooter::get_cur_freq() const {
    return speed->get_frequency();
}

double shooter::get_set_speed(double a) const {
    return rps_to_ballspeed(setpoint, a);
}

double shooter::get_set_freq() const {
    return setpoint;
}
