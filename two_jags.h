#ifndef TWO_JAGS_H_INC
#define TWO_JAGS_H_INC

#include <PIDOutput.h>

class two_jags : public PIDOutput {
private:
    Jaguar* jag1;
    Jaguar* jag2;
    two_jags() {} //SHOULD NEVER EVER BE CALLED
public:
    two_jags(Jaguar& a, Jaguar& b) : jag1(&a), jag2(&b) {}
    void PIDWrite(float output) {
        jag1->PIDWrite(output);
        jag2->PIDWrite(output);
    }
};

#endif
