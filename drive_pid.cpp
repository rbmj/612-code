#include "612.h"
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif
#include <RobotDrive.h>
#include "drive_pid.h"

drive_pid::drive_pid(SpeedController& j1, SpeedController& j2, drive_pid_side_t s, bool r) : side(s), reverse(r) {
	jag1 = static_cast<SPEEDCONTROLLER*>(&j1);
	jag2 = static_cast<SPEEDCONTROLLER*>(&j2);
}

void drive_pid::PIDWrite(float output) {
    jag1->PIDWrite(output*(reverse?-1.0:1.0));
    jag2->PIDWrite(output*(reverse?-1.0:1.0));
}
