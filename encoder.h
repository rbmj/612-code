#ifndef ENCODER_H_INC
#define ENCODER_H_INC

#include <Encoder.h>
#include <PIDController.h>

//I don't normally use CamelCase, but meh.

//there also needs to be a gurantee during Teleop that if the left or right
//joystick has a X or Y axis value v and std::fabs(v) > 0.25, then we MUST
//disable PID control and return everything to the driver.

class EncoderWheels {
private:
    EncoderWheels(Encoder&, Encoder&);
    static encoder_wheels * instance;
    static const double wheel_radius;
    static const unsigned ticks_per_rev;
    static const double drive_reduction;
    bool enabled;
    double setpoint;
    Encoder& left;
    Encoder& right;
    pid_controller * pidcontrol;
public:
    static void Init(Encoder&, Encoder&);
    static EncoderWheels& GetInstance();
    void Enable();
    void Disable();
    void SetDistance(double);
    void AtTarget();
    void GetCurDistance();
    void GetSetDistance();
};

#endif
