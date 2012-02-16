#ifndef TWO_JAGS_H_INC
#define TWO_JAGS_H_INC

#include <PIDOutput.h>
#include <Jaguar.h>

class two_jags : public PIDOutput {
private:
    Jaguar* jag1;
    Jaguar* jag2;
    two_jags() {} //SHOULD NEVER EVER BE CALLED
public:
    two_jags(Jaguar&, Jaguar&);
    void PIDWrite(float);
};

#endif
