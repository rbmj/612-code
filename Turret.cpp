#include "Turret.h"
#include "vision_alt.h"
#include "ports.h"

#include <PIDController.h>
#if DEBUG_612
#include <SmartDashboard/SendablePIDController.h>
#endif

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

Turret::Turret(Jaguar& rotate, Jaguar& winch, Jaguar& launcha, Jaguar& launchb, Counter& count) :
    launcher_jags(launcha, launchb), launch_count(count)
{
    rotation_jag = &rotate;
    winch_jag = &winch;
    
    //allocate PID Control
#if DEBUG_612
    launch_control = new SendablePIDController(PID_P, PID_I, PID_D, &count, &launcher_jags);
    key = "LAUNCH_PID"; //TODO: If necessary, set this to be unique
    SmartDashboard::GetInstance()->PutData(key, launch_control);
#else
    launch_control = new SendablePIDController(PID_P, PID_I, PID_D, &count, &launcher_jags);
#endif
    //done!
}

Turret::~Turret() {
    delete launch_control;
}

void Turret::Align(target& t) {
    cur_target = &t;
}

