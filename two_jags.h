#ifndef TWO_JAGS_H_INC
#define TWO_JAGS_H_INC

#include "612.h"
#include <vxWorks.h>
#include <PIDOutput.h>
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif

class two_jags : public PIDOutput {
private:
    SPEEDCONTROLLER* jag1;
    SPEEDCONTROLLER* jag2;
    two_jags() {} //SHOULD NEVER EVER BE CALLED
public:
    two_jags(SpeedController&, SpeedController&);
    virtual ~two_jags() {}
    float speed;
    void PIDWrite(float);
    void Set(float, UINT8 = 0);
    void reset();
};

#endif
