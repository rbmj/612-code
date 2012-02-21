#ifndef WINCH_H_INC
#define WINCH_H_INC

#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>
#include "pid_controller.h"

class winch {
public:
    winch(Jaguar&, AnalogChannel&, DigitalInput&);
    ~winch();
    void enable();
    void disable();
    bool is_enabled();
    void set_angle(double);
    void update();
private:
    Jaguar * jag;
    AnalogChannel * pot;
    DigitalInput * limit;
//    pid_controller * pid;
//    winch_motor * motor;
    bool enabled;
    float desired_pot_voltage;
    static double launch_angle_to_voltage(double);
};

#endif
