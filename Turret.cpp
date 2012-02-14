#include "Turret.h"
#include "vision_alt.h"
#include "ports.h"

#include <PIDController.h>
#include <Counter.h>
#if DEBUG_612
#include <SmartDashboard/SendablePIDController.h>
#include <SmartDashboard/SmartDashboard.h>
#endif

#include "two_jags.h"
#include "612.h"

#include <cstdlib>


const int Zero_Max = 4;
const int Zero_Min = -1;
const int Left = 0;
const int Right = 1;
const float Turn_Speed = 0.1;

//PID Gains
const double PID_P = 0.0;
const double PID_I = 0.0;
const double PID_D = 0.0;

Turret::Turret(Jaguar& rotate, Jaguar& winch, Jaguar& launcha, Jaguar& launchb, Counter& count) {
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

Turret::~Turret() {
    delete launch_control;
}

void Turret::Align(target& t) {
    cur_target = &t;
}

