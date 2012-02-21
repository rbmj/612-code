#ifndef SHOOTER_H_INC
#define SHOOTER_H_INC

#include "launch_counter.h"
#include "two_jags.h"
#include "pid_controller.h"
#include <Counter.h>
#include <Timer.h>

class shooter {
public:
    shooter(Counter&, Jaguar&, Jaguar&);
    ~shooter();
    void set_speed(double);
    void set_freq(double);
    double get_cur_speed() const;
    double get_cur_freq() const;
    double get_set_speed() const;
    double get_set_freq() const;
    void enable();
    void disable();
private:
    shooter() {}
    pid_controller * control;
    launch_counter * speed;
    two_jags * jags;
    double setpoint;
    bool enabled;
    bool timer_started;
    bool setpoint_set;
    bool arrived_at_speed;
    Timer launch_time;
    void update();
    static double ballspeed_to_rps(double);
    static double rps_to_ballspeed(double);
    static void update_help(void*);
};

#endif
