#include <RobotDrive.h>
#include "drive_pid.h"
#include "ports.h"

drive_pid::drive_pid(drive_jaguar& j1, drive_jaguar& j2, drive_pid_side_t s) : jag1(&j1), jag2(&j2), side(s) {
}

void drive_pid::PIDWrite(float output) {
    jag1->jag.PIDWrite(output*(jag1->reverse?-1:1));
    jag2->jag.PIDWrite(output*(jag2->reverse?-1:1));
}
