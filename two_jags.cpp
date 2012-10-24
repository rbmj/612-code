#include "612.h"
#include <vxWorks.h>
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif
#include "two_jags.h"
#include "utility.h"

two_jags::two_jags(SpeedController& a, SpeedController& b) : speed(0.0) {
	jag1 = static_cast<SPEEDCONTROLLER*>(&a);
	jag2 = static_cast<SPEEDCONTROLLER*>(&b);
}

void two_jags::PIDWrite(float output) {
    speed = coerce(output + speed, -1.0f, 1.0f);
    jag1->PIDWrite(speed);
    jag2->PIDWrite(speed);
}

void two_jags::Set(float value, UINT8 sync) {
    speed = coerce(value, -1.0f, 1.0f);
    jag1->Set(value, sync);
    jag2->Set(value, sync);
}

void two_jags::reset() {
    Set(0.0);
}
