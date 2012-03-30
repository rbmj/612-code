#ifndef SHOOTER_H_INC
#define SHOOTER_H_INC

#include "launch_counter.h"
#include "two_jags.h"
#include "pid_controller.h"
#include <Counter.h>
#include <Timer.h>

const float WHEEL_TOLERANCE = 1.5; //percent
const float NUM_SHOT_TOLERANCE = 10.0;
const int MAX_RPS = 75;

class shooter {
public:
    shooter(Counter&, Jaguar&, Jaguar&);
    ~shooter();
    void set_speed(double, double);
    void set_freq(double);
    double get_cur_speed(double) const;
    double get_cur_freq() const;
    double get_set_speed(double) const;
    double get_set_freq() const;
    void enable();
    void disable();
    static double ballspeed_to_rps(double,double);
    static double rps_to_ballspeed(double,double);
    bool at_setpoint();
    unsigned int get_num_shot();
    void reset_num_shot();
private:
    shooter() {}
	void update_num_shot ();
	double last_setpoint;
	bool reached_target;
    unsigned int num_shot;
    pid_controller * control;
    launch_counter * speed;
    two_jags * jags;
    double setpoint;
    bool enabled;
    bool timer_started;
    bool setpoint_set;
    bool arrived_at_speed;
    bool ball_incremented;
    Timer launch_time;
    void update();
    static void update_help(void*);
};

#endif
