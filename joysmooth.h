#ifndef JOYSMOOTH__INC
#define JOYSMOOTH_H_INC

#include "612.h"s
#include <bitset>
#include <GenericHID>

class joysmooth : public GenericHID {
private:
    const static int holdback;
    const static int numbuttons;
    const static int numaxes;
    std::bitset<holdback> buttons[numbuttons]; //2d array buttons[num][hist]
    GenericHID * joy;
    float axes[numaxes];
public:
    joysmooth(GenericHID* g) : joy(g) {}
    joysmooth(GenericHID& g) : joy(&g) {}
    void update();

    //implement GenericHID Interface
    float GetX(JoystickHand hand=kRightHand);
    float GetY(JoystickHand hand=kRightHand);
    float GetZ();
    float GetTwist();
    float GetThrottle();
    float GetRawAxis();
    bool GetTrigger(JoystickHand hand=kRightHand); //button 1
    bool GetTop(JoystickHand hand=kRightHand); //button 2
    bool GetBumper(); //just return joy->GetBumper() straight
    bool GetRawButton(uinteger);

};

#endif
