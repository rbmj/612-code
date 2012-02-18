#include "pid_controller.h"
#include "612.h"
#include <PIDController.h>
#include <PIDOutput.h>
#include <PIDSource.h>
#if DEBUG_612
#include <SmartDashboard/SendablePIDController.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cstdio>
#endif

const char format[] = "pid_controller@0x%X";
const unsigned slen = sizeof(format) + (2 * sizeof(void*)) - 2;
//slen is size of format string + 2 bytes per pointer byte minus two for the %X

pid_controller::pid_controller(float p, float i, float d, PIDSource* s, PIDOutput* o, float f)
    : PIDController(p, i, d, s, o, f) //shouldn't do anything - just shut up gcc
{
#if DEBUG_612
    pidobj = new SendablePIDController(p, i, d, s, o, f);
    key = new char[slen];
    std::sprintf(key, &(format[0]), (unsigned long)this);
    SmartDashboard::GetInstance()->PutData(key, (SendablePIDController*)pidobj);
    //NOTE: THE ABOVE POINTER CAST IS VERY DANGEROUS!!!
#else
    pidobj = new PIDController(p, i, d, s, o, f);
#endif
}

pid_controller::~pid_controller() {
#if DEBUG_612
    delete[] key;
#endif
    delete pidobj;
}
