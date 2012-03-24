#ifndef ENCODER_H_INC
#define ENCODER_H_INC

#include <Encoder.h>
#include <PIDController.h>
#include "ports.h"
#include "pid_controller.h"
#include "drive_pid.h"

//I don't normally use CamelCase, but meh.

//there also needs to be a gurantee during Teleop that if the left or right
//joystick has a X or Y axis value v and std::fabs(v) > 0.25, then we MUST
//disable PID control and return everything to the driver.

//the gear reduction from the encoder shaft to the drive shafts is 38:60
//the wheels are about 8" in diameter.
//the encoders on those wheels are 250 ticks per revolution
//therefore distance = ticks * (250/(2pi)) * (38/60) * (8/2)

const double WHEEL_RADIUS = 8.0/2;
const double TICKS_PER_REV = 250;
const double DRIVE_REDUCTION = 38.0/60;

class EncoderWheels {
private:
    EncoderWheels(Encoder&, Encoder&, drive_jaguar&, drive_jaguar&, drive_jaguar&, drive_jaguar&);
    static EncoderWheels * instance;
    static const double wheel_radius;
    static const unsigned ticks_per_rev;
    static const double drive_reduction;
    bool enabled;
    double setpoint;
    Encoder& encoder_left;
    Encoder& encoder_right;
    drive_pid* drive_pid_obj_left;
    drive_pid* drive_pid_obj_right;
    pid_controller* pid_left;
    pid_controller* pid_right;
public:
    enum distance_side_t {
        DISTANCE_LEFT = 0x1,
        DISTANCE_RIGHT = 0x2,
        DISTANCE_AVG = DISTANCE_LEFT | DISTANCE_RIGHT
    };
    static void Init(Encoder&, Encoder&, drive_jaguar&, drive_jaguar&, drive_jaguar&, drive_jaguar&);
    static EncoderWheels& GetInstance();
    void Enable();
    void Disable();
    void SetDistance(double);
    bool AtTarget();
    double GetCurDistance(distance_side_t);
    double GetSetDistance();
};

#endif
