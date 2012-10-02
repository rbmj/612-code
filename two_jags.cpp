#include <vxWorks.h>
#include "two_jags.h"
#include "612.h"
#include "utility.h"
#include <Jaguar.h>

two_jags::two_jags(Jaguar& a, Jaguar& b) : jag1(&a), jag2(&b), speed(0.0) {}

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
