#include "two_jags.h"
#include <Jaguar.h>

two_jags::two_jags(Jaguar& a, Jaguar& b) : jag1(&a), jag2(&b) {}

void two_jags::PIDWrite(float output) {
    jag1->PIDWrite(output);
    jag2->PIDWrite(output);
}
