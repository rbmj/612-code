#ifndef SHOOTER_H_INC
#define SHOOTER_H_INC

#include "launch_counter.h"
#include "two_jags.h"
#include <Counter.h>
#include <PIDController.h>

class shooter {
public:
    shooter(Counter&, Jaguar&, Jaguar&);
    ~shooter();
    void set_speed(float);
    void enable();
    void disable();
private:
    shooter() {}
    PIDController * control;
    launch_counter * speed;
    two_jags * jags;
    
#if DEBUG_612
    const char * key;
#endif
};

#endif
