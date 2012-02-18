#include "turret.h"
#include "vision_alt.h"
#include "ports.h"

#include <PIDController.h>
#include <Counter.h>
#include <AnalogChannel.h>

#include "612.h"
#include "ports.h"
#include "two_jags.h"
#include "shooter.h"
#include "visionalg.h"
#include "trajectory.h"

#include <cstdlib>

turret::turret(
    Jaguar& rotate,
    Jaguar& winch_jag,
    Jaguar& launcha,
    Jaguar& launchb,
    Counter& count,
    AnalogChannel& pot,
    DigitalInput& winch_limit,
    DigitalInput& turret_left,
    DigitalInput& turret_mid,
    DigitalInput& turret_right)
{
    lazy_susan = new turntable(rotate, turret_left, turret_mid, turret_right);
    winch_obj = new winch(winch_jag, pot, winch_limit);
    shooter_wheels = new shooter(count, launcha, launchb);
    
    cur_target = NULL;
    enabled = false;
    just_enabled = false;
}

turret::~turret() {
    delete shooter_wheels;
    delete winch_obj;
    delete lazy_susan;
}

void turret::align(target& t) {
    cur_target = &t;
}

void turret::enable() {
    enabled = true;
    just_enabled = true;
}

void turret::disable() {
    enabled = false;
}

void turret::update_help(void * obj) {
    ((turret*)obj)->update();
}

void turret::update() {
    if (cur_target != NULL && cur_target->valid() && enabled) {
        if (just_enabled) {
            winch_obj->enable();
            shooter_wheels->enable();
            lazy_susan->enable();
            just_enabled = false;
        }
        if (cur_target->fresh()) {
            trajectory traj = calculate_trajectory(*cur_target);
            lazy_susan->new_offset(cur_target->x_offset(), RESOLUTION().X()/2);
            shooter_wheels->set_speed(traj.velocity);
            winch_obj->set_angle(traj.angle);
        }
    }
    else {
        winch_obj->disable();
        shooter_wheels->disable();
        lazy_susan->disable();
    }
}
