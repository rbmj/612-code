#include "turret.h"
#include "vision_alt.h"
#include "ports.h"

#include <PIDController.h>
#include <Counter.h>
#if DEBUG_612
#include <SmartDashboard/SendablePIDController.h>
#include <SmartDashboard/SmartDashboard.h>
#endif

#include "612.h"
#include "ports.h"
#include "two_jags.h"
#include "shooter.h"

#include <cstdlib>

turret::turret(Jaguar& rotate, Jaguar& winch, Jaguar& launcha, Jaguar& launchb, Counter& count) {
    rotation_jag = &rotate;
    winch_jag = &winch;
    
    shooter_wheels = new shooter(count, launcha, launchb);
    
    cur_target = NULL;
}

turret::~turret() {
    delete shooter_wheels;
}

void turret::align(target& t) {
    cur_target = &t;
}
