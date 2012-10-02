#ifndef TWO_JAGS_H_INC
#define TWO_JAGS_H_INC

#include <vxWorks.h>
#include <PIDOutput.h>
#include <Jaguar.h>
#include "612.h"

class two_jags : public PIDOutput {
private:
    Jaguar* jag1;
    Jaguar* jag2;
    two_jags() {} //SHOULD NEVER EVER BE CALLED
public:
    two_jags(Jaguar&, Jaguar&);
    virtual ~two_jags() {}
    float speed;
    void PIDWrite(float);
    void Set(float, UINT8 = 0);
    void reset();
};

#endif
