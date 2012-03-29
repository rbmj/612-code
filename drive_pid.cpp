#include <Jaguar.h>
#include <RobotDrive.h>
#include "drive_pid.h"

drive_pid::drive_pid(Jaguar& j1, Jaguar& j2, drive_pid_side_t s, bool r) : jag1(&j1), jag2(&j2), side(s), reverse(r) {
}

void drive_pid::PIDWrite(float output) {
    jag1->PIDWrite(output*(reverse?-1.0:1.0));
    jag2->PIDWrite(output*(reverse?-1.0:1.0));
}
