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

#include <cstdlib>


const int ZERO_MAX = 4;
const int ZERO_MIN = -1;
const float TURN_SPEED = 0.1;

//PID Gains
const double PID_P = 0.0;
const double PID_I = 0.0;
const double PID_D = 0.0;

turret::turret(Jaguar& rotate, Jaguar& winch, Jaguar& launcha, Jaguar& launchb, Counter& count) {
    rotation_jag = &rotate;
    winch_jag = &winch;
    
    launcher_jags = new two_jags(launcha, launchb);
    launch_speed = new launch_counter(count);
    
    //allocate PID Control
#if DEBUG_612
    launch_control = new SendablePIDController(PID_P, PID_I, PID_D, launch_speed, launcher_jags);
    key = "LAUNCH_PID"; //TODO: If necessary, set this to be unique
    SmartDashboard::GetInstance()->PutData(key, (SendablePIDController*)launch_control);
    //NOTE: THE ABOVE LINE IS DANGEROUS!!!!!!!!!!
#else
    launch_control = new PIDController(PID_P, PID_I, PID_D, launch_speed, launcher_jags);
#endif
    //done!
}

turret::~turret() {
    delete launch_control;
}

void turret::align(target& t) {
    cur_target = &t;
}

void turret:turn(DIRECTION direction) {
    if(direction == LEFT) {
        rotation_jag.Set(TURN_SPEED);
    }
    else { // RIGHT is implied (not assumed)
        rotation_jag.Set(-TURN_SPEED);
    }
}

//turret robot_turret(turret_rotation_jag, turret_winch_jag, left_launcher_jag, right_launcher_jag
