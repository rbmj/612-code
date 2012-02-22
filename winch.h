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
    bool is_enabled() const;
    void set_angle(float);
    float get_cur_angle() const;
    float get_set_angle() const;
    void update();
    void manual_control(direction_t);
private:
    Jaguar * jag;
    AnalogChannel * pot;
    DigitalInput * limit;
    bool enabled;
    float desired_angle;
    float desired_pot_voltage;
    static float launch_angle_to_voltage(float);
    static float voltage_to_launch_angle(float);
};

#endif
