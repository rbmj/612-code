#ifndef JOYSMOOTH_H_INC
#define JOYSMOOTH_H_INC

#include "612.h"
#include <bitset>
#include <GenericHID>

class joysmooth : public GenericHID {
private:
    const static int HOLDBACK=2;
    const static int NUMBUTTONS;
    const static int NUMAXES=5;
    const static unsigned int X_AXIS=0;
    const static unsigned int Y_AXIS=1;
    const static unsigned int Z_AXIS=2;
    const static unsigned int TWIST_AXIS=3;
    const static unsigned int THROTTLE_AXIS=4;
    std::bitset<holdback> buttons[numbuttons]; //2d array buttons[num][hist]
    GenericHID * joy;
    float axes[NUMAXES];
public:
    joysmooth(GenericHID* g);
    joysmooth(GenericHID& g);
    ~joysmooth();
    void update();

    //implement GenericHID Interface
    float GetX();
    float GetY();
    float GetZ();
    float GetTwist();
    float GetThrottle();
    float GetRawAxis(uninteger);
    bool GetTrigger(); //button 1
    bool GetTop(); //button 2
    bool GetBumper(); //just return joy->GetBumper() straight
    bool GetRawButton(uinteger);

};

#endif
