#include <cmath>
#include <RobotDrive.h>
#include "encoder.h"
#include "ports.h"
#include "trajectory.h"
#include "pid_controller.h"
#include "two_jags.h"

// PID values from the almighty Wikipedia
const double DRIVETRAIN_PID_P = 2.0;
const double DRIVETRAIN_PID_I = 0.5;
const double DRIVETRAIN_PID_D = 0;
//const double DRIVETRAIN_TOLERANCE = 1.0;
//const double DRIVETRAIN_SPEED = 0.7;

EncoderWheels * EncoderWheels::instance = NULL;

EncoderWheels::EncoderWheels(Encoder& el, Encoder& er, drive_jaguar& jag_fl, drive_jaguar& jag_fr, drive_jaguar& jag_rl, drive_jaguar& jag_rr) : encoder_left(el), encoder_right(er) {
    enabled = false;
    setpoint = 0.0;
    drive_pid_obj_left = new drive_pid(jag_fl, jag_rl, drive_pid::DRIVE_PID_LEFT);
    drive_pid_obj_right = new drive_pid(jag_fr, jag_rr, drive_pid::DRIVE_PID_RIGHT);
    pid_left = new pid_controller(DRIVETRAIN_PID_P, DRIVETRAIN_PID_I, DRIVETRAIN_PID_D, &encoder_left, drive_pid_obj_left);
    pid_right = new pid_controller(DRIVETRAIN_PID_P, DRIVETRAIN_PID_I, DRIVETRAIN_PID_D, &encoder_right, drive_pid_obj_right);
}

void EncoderWheels::Init(Encoder& el, Encoder& er, drive_jaguar& jag_fl, drive_jaguar& jag_fr, drive_jaguar& jag_rl, drive_jaguar& jag_rr) {
    instance = new EncoderWheels(el, er, jag_fl, jag_fr, jag_rl, jag_rr);
    //don't need to delete as this object will last the lifetime of the program,
    //and the memory will be reclaimed when the process ends.  Only call this
    //once though or it will be VERY bad
}

EncoderWheels& EncoderWheels::GetInstance() {
    //if you don't call init this will blow up in your face
    return *instance;
}

//there's a bool here but it will probably be more like a PID enable
void EncoderWheels::Enable() {
    pid_left->Enable();
    encoder_left.Start();
    encoder_left.Reset();
    encoder_right.Start();
    encoder_right.Reset();
    enabled = true;
}

void EncoderWheels::Disable() {
    pid_left->Disable();
    encoder_left.Stop();
    encoder_right.Stop();
    enabled = false;
}

void EncoderWheels::SetDistance(double distance) {
    setpoint = distance;
    pid_left->SetSetpoint(distance);
    pid_right->SetSetpoint(distance);
/*    if(AtTarget()) {
        drive.Drive(0,0);
    } else {
        drive.Drive(DRIVETRAIN_SPEED,0);
    }*/
}

bool EncoderWheels::AtTarget() {
    return pid_left->OnTarget() && pid_right->OnTarget();
//    return std::fabs(GetCurDistance()-setpoint) > DRIVETRAIN_TOLERANCE;
}

double EncoderWheels::GetCurDistance(distance_side_t side) {
    double distance_ret=0;
    float axes=4.0; // assume four, no way to find type of encoder right now
    if(side & DISTANCE_LEFT) {
        double left_ticks=encoder_left.Get()/axes;
        double left_distance=left_ticks * (TICKS_PER_REV/(2*pi)) * DRIVE_REDUCTION * WHEEL_RADIUS;
        left_distance/=-360;
        distance_ret+=left_distance;
    }
    if(side & DISTANCE_RIGHT) {
        double right_ticks=encoder_right.Get()/axes;
        double right_distance=right_ticks * (TICKS_PER_REV/(2*pi)) * DRIVE_REDUCTION * WHEEL_RADIUS;
        right_distance/=-360;
        distance_ret+=right_distance;
    }
    if(side == DISTANCE_AVG) {
        distance_ret/=2.0;
    }
    return distance_ret;
}

double EncoderWheels::GetSetDistance() {
    return setpoint;
}
