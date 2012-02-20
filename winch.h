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
    void set_angle(double);
private:
    Jaguar * jag;
    AnalogChannel * pot;
    DigitalInput * limit;
    pid_controller * pid;
    static double angle_to_voltage(double);
    static double launch_angle_to_winch_angle(double);
};

#endif
