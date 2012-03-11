#include "encoder.h"
#include "ports.h"
#include "trajectory.h"
//trajectory.h for pi
#include <RobotDrive.h>

const double DRIVETRAIN_PID_P = ; //TODO: Get Value (Can't find last year's :/)
const double DRIVETRAIN_PID_I = ; //TODO: Get Value
const double DRIVETRAIN_PID_D = ; //TODO: Get Value

//the gear reduction from the encoder shaft to the drive shafts is 38:60
//the wheels are about 8" in diameter.
//the encoders on those wheels are 250 ticks per revolution

EncoderWheels * EncoderWheels::instance = NULL;
const double EncoderWheels::wheel_radius = 8.0/2;
const double EncoderWheels::ticks_per_rev = 250;
const double EncoderWheels::drive_reduction = 38.0/60;

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
}

void EncoderWheels::Disable() {
    enabled = false;
}

    
