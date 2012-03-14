#include <cmath>
#include "encoder.h"
#include "ports.h"
#include "trajectory.h"
#include <RobotDrive.h>

/*const double DRIVETRAIN_PID_P = ; //TODO: Get Value (Can't find last year's :/)
const double DRIVETRAIN_PID_I = ; //TODO: Get Value
const double DRIVETRAIN_PID_D = ; //TODO: Get Value*/
const double DRIVETRAIN_TOLERANCE = 1.0;
const double DRIVETRAIN_SPEED = 0.7;
// no pid right now

//the gear reduction from the encoder shaft to the drive shafts is 38:60
//the wheels are about 8" in diameter.
//the encoders on those wheels are 250 ticks per revolution

EncoderWheels * EncoderWheels::instance = NULL;

//therefore distance = ticks * (250/(2pi)) * (38/60) * (8/2)

EncoderWheels::EncoderWheels(Encoder& l, Encoder& r) : left(l), right(r) {
    enabled = false;
    setpoint = 0.0;
    //TODO: Finish
}

void EncoderWheels::Init(Encoder& l, Encoder& r) {
    instance = new EncoderWheels(l, r);
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
    enabled = true;
    left.Start();
    left.Reset();
    right.Start();
    right.Reset();
}

void EncoderWheels::Disable() {
    enabled = false;
    left.Stop();
    left.Stop();
}

void EncoderWheels::SetDistance(double distance) {
    setpoint = distance;
    if(AtTarget()) {
        drive.Drive(0,0);
    } else {
        drive.Drive(DRIVETRAIN_SPEED,0);
    }
}

bool EncoderWheels::AtTarget() {
    return std::fabs(GetCurDistance()-setpoint) > DRIVETRAIN_TOLERANCE;
}

double EncoderWheels::GetCurDistance() {
    float axes=4.0; // assume four, no way to find type of encoder right now
    double ticks=left.Get()/axes;
    double left_distance=ticks * (TICKS_PER_REV/(2*pi)) * DRIVE_REDUCTION * WHEEL_RADIUS;
    left_distance/=-360;
    return left_distance;
}

double EncoderWheels::GetSetDistance() {
    return setpoint;
}
