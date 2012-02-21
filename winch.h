#ifndef WINCH_H_INC
#define WINCH_H_INC

#include <Jaguar.h>
#include <AnalogChannel.h>
#include <DigitalInput.h>
#include "pid_controller.h"

class winch {
public:
    enum direction_t {
        UP,
        DOWN,
        OFF
    };
    winch(Jaguar&, AnalogChannel&, DigitalInput&);
    ~winch();
    void enable();
    void disable();
    bool is_enabled();
    void set_angle(double);
    void update();
    void manual_control(direction_t);
private:
    Jaguar * jag;
    AnalogChannel * pot;
    DigitalInput * limit;
    bool enabled;
    float desired_pot_voltage;
    static double launch_angle_to_voltage(double);
    static double voltage_to_launch_angle(double);
};

#endif
