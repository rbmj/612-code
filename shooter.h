#ifndef SHOOTER_H_INC
#define SHOOTER_H_INC

#include "launch_counter.h"
#include "two_jags.h"
#include "pid_controller.h"
#include <Counter.h>

class shooter {
public:
    shooter(Counter&, Jaguar&, Jaguar&);
    ~shooter();
    void set_speed(double);
    void enable();
    void disable();
private:
    shooter() {}
    pid_controller * control;
    launch_counter * speed;
    two_jags * jags;
    static double ballspeed_to_rps(double);
};

#endif
