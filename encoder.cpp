#include <cmath>
#include <RobotDrive.h>
#include "encoder.h"
#include "ports.h"
#include "trajectory.h"
#include "pid_controller.h"
#include "update.h"

// PID values from the almighty Wikipedia
// TODO retune PID for Suzie
const double DRIVETRAIN_PID_P = 0.24; // 0.025;
const double DRIVETRAIN_PID_I = 0.000; // 0.0005;
const double DRIVETRAIN_PID_D = 0;
const double DRIVETRAIN_PID_TOLERANCE = 0.75;
//const double DRIVETRAIN_TOLERANCE = 1.0;
//const double DRIVETRAIN_SPEED = 0.7;
// TODO Joystick + encoder pid driving at the same time
// TODO decide what is reverse and what isn't!
const int RESET               = 1;

EncoderWheels * EncoderWheels::instance = NULL;

EncoderWheels::EncoderWheels(Encoder& el, Encoder& er, Jaguar& jag_fl, Jaguar& jag_fr, Jaguar& jag_rl, Jaguar& jag_rr) : encoder_left(el), encoder_right(er) {
    enabled = false;
    setpoint = 0.0;
    encoder_left.SetDistancePerPulse(2 * pi * DRIVE_REDUCTION * WHEEL_RADIUS / 360 * -1.0);
    encoder_right.SetDistancePerPulse(2 * pi * DRIVE_REDUCTION * WHEEL_RADIUS / 360);
    encoder_left.SetPIDSourceParameter(Encoder::kDistance);
    encoder_right.SetPIDSourceParameter(Encoder::kDistance);
    drive_pid_obj_left = new drive_pid(jag_fl, jag_rl, drive_pid::SIDE_LEFT, true);
    drive_pid_obj_right = new drive_pid(jag_fr, jag_rr, drive_pid::SIDE_RIGHT, false);
    pid_left = new pid_controller(DRIVETRAIN_PID_P, DRIVETRAIN_PID_I, DRIVETRAIN_PID_D, &encoder_left, drive_pid_obj_left);
    pid_right = new pid_controller(DRIVETRAIN_PID_P, DRIVETRAIN_PID_I, DRIVETRAIN_PID_D, &encoder_right, drive_pid_obj_right);
    pid_left->SetTolerance(DRIVETRAIN_PID_TOLERANCE);
    pid_right->SetTolerance(DRIVETRAIN_PID_TOLERANCE);
    pid_left->SetOutputRange(-0.5, 0.5);
    pid_right->SetOutputRange(-0.5, 0.5);
    registry().register_func(EncoderWheels::update_help, (void*)this);
}

void EncoderWheels::update_help(void* obj) {
    ((EncoderWheels*)obj)->update();
}

void EncoderWheels::update() {
    if(AtTarget()) {
        std::printf("------------TARGET REACHED-----------------\n");
        Disable();
    }
    if(left_joystick.GetRawButton(RESET)) {
        std::printf("............RESET ENCODERS.............\n");
        Disable();
        Enable();
    }
}

void EncoderWheels::Init(Encoder& el, Encoder& er, Jaguar& jag_fl, Jaguar& jag_fr, Jaguar& jag_rl, Jaguar& jag_rr) {
    instance = new EncoderWheels(el, er, jag_fl, jag_fr, jag_rl, jag_rr);
    //don't need to delete as this object will last the lifetime of the program,
    //and the memory will be reclaimed when the process ends.  Only call this
    //once though or it will be VERY bad
}

EncoderWheels& EncoderWheels::GetInstance() {
    //if you don't call init this will blow up in your face
    return *instance;
}

double EncoderWheels::InchesToTicks(double inches, int axes=4) {
    double ticks = inches*360;
    ticks = ticks / (TICKS_PER_REV/(2*pi)) / DRIVE_REDUCTION / WHEEL_RADIUS;
    ticks *= axes;
    return ticks;
}

//there's a bool here but it will probably be more like a PID enable
void EncoderWheels::Enable() {
    if(std::fabs(setpoint)>1.0) {
        drive.SetSafetyEnabled(false);
        pid_left->Enable();
        pid_right->Enable();
    }
    encoder_left.Start();
    encoder_left.Reset();
    encoder_right.Start();
    encoder_right.Reset();
    enabled = true;
}

void EncoderWheels::Disable() {
    pid_left->Disable();
    pid_right->Disable();
    drive.SetSafetyEnabled(true);
    encoder_left.Stop();
    encoder_right.Stop();
    setpoint = 0.0;
    enabled = false;
}

bool EncoderWheels::IsEnabled() {
    return enabled;
}

void EncoderWheels::SetDistance(double distance) {
    Disable();
    distance /= 1.7;
    setpoint = distance;
    pid_left->SetSetpoint(setpoint);
    std::printf("set pid left to %f\n", distance);
    pid_right->SetSetpoint(setpoint);
    std::printf("set pid right to %f\n", distance);
    Enable();
/*    if(AtTarget()) {
        drive.Drive(0,0);
    } else {
        drive.Drive(DRIVETRAIN_SPEED,0);
    }*/
}

bool EncoderWheels::AtTarget() {
    bool TargetReached = pid_left->OnTarget() && pid_right->OnTarget();
    return TargetReached;
//    return std::fabs(GetCurDistance()-setpoint) > DRIVETRAIN_TOLERANCE;
}

double EncoderWheels::GetCurDistance(distance_side_t side) {
    double distance_ret=0;
//    int axes=4; // assume four, no way to find type of encoder right now
    if(side & DISTANCE_LEFT) {
/*        double left_ticks=encoder_left.Get()*1.0/axes;
        std::printf("left_ticks = %f", left_ticks);
        double left_distance=left_ticks * (TICKS_PER_REV/(2*pi)) * DRIVE_REDUCTION * WHEEL_RADIUS;
        left_distance/=-360;
        distance_ret+=left_distance;*/
        distance_ret+=encoder_left.GetDistance()*1.7;
    }
    if(side & DISTANCE_RIGHT) {
/*        double right_ticks=encoder_right.Get()*1.0/axes;
        double right_distance=right_ticks * (TICKS_PER_REV/(2*pi)) * DRIVE_REDUCTION * WHEEL_RADIUS;
        right_distance/=-360;
        distance_ret+=right_distance;*/
        distance_ret+=encoder_right.GetDistance()*1.7;
    }
    if(side == DISTANCE_AVG) {
        distance_ret/=2.0;
    }
    return distance_ret;
}

double EncoderWheels::GetSetDistance() {
    return setpoint*1.7;
}
