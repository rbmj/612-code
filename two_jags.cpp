#include "two_jags.h"
#include "612.h"
#include <Jaguar.h>

two_jags::two_jags(Jaguar& a, Jaguar& b) : jag1(&a), jag2(&b), speed(0.0) {}

void two_jags::PIDWrite(float output) {
    speed += output;
    jag1->PIDWrite(speed);
    jag2->PIDWrite(speed);
}

void two_jags::Set(float value, ucharacter sync) {
    speed = value;
    jag1->Set(value, sync);
    jag2->Set(value, sync);
}
