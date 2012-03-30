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
    enabled = true;
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
    winch_obj->enable();
    shooter_wheels->enable();
    lazy_susan->enable();
}

void turret::disable() {
    enabled = false;
}

void turret::update_help(void * obj) {
    ((turret*)obj)->update();
}

void turret::update() {
    if (enabled) {
        if (cur_target != NULL && cur_target->valid()) {
            //auto target mode
            if (cur_target->fresh()) {
                trajectory traj = projected_trajectory(cur_target);
                lazy_susan->new_offset(cur_target->x_offset(), RESOLUTION().X()/2);
                shooter_wheels->set_speed(traj.velocity, traj.angle);
                winch_obj->set_angle(traj.angle);
            }
        }
        else {
            //other mode
        }
    }
    else {
        winch_obj->disable();
        shooter_wheels->disable();
        lazy_susan->disable();
    }
}

shooter& turret::Shooter() {
    return *shooter_wheels;
}

winch& turret::Winch() {
    return *winch_obj;
}

turntable& turret::Susan() {
    return *lazy_susan;
}
