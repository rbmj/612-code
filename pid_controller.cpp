#include "pid_controller.h"
#include "612.h"
#include <PIDController.h>
#include <PIDOutput.h>
#include <PIDSource.h>

const char format[] = "pid_controller@0x%lX";
const unsigned slen = sizeof(format) + (2 * sizeof(void*)) - 2;
//slen is size of format string + 2 bytes per pointer byte minus two for the %X

pid_controller::pid_controller(float p, float i, float d, PIDSource* s, PIDOutput* o, float f)
    : PIDController(p, i, d, s, o, f) //shouldn't do anything - just shut up gcc
{
    pidobj = new PIDController(p, i, d, s, o, f);
}

pid_controller::~pid_controller() {
    delete pidobj;
}
