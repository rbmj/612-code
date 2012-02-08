#ifndef JOYSMOOTH_H_INC
#define JOYSMOOTH_H_INC

#include <bitset>
#include <GenericHID.h>
#include "612.h"

class joysmooth : public GenericHID {
private:
    const static int HOLDBACK = 2;
    const static int NUMBUTTONS = 11;
    const static int NUMAXES = 5;
    std::bitset<HOLDBACK> buttons[NUMBUTTONS]; //2d array buttons[num][hist]
    GenericHID * joy;
    float axes[NUMAXES];
    
public:
    enum axis_t {
        X_AXIS = 0,
        Y_AXIS,
        Z_AXIS,
        TWIST_AXIS,
        THROTTLE_AXIS
    };
    joysmooth(GenericHID* g);
    joysmooth(GenericHID& g);
    ~joysmooth();
    void update();

    //implement GenericHID Interface
    float GetX(JoystickHand = kRightHand);
    float GetY(JoystickHand = kRightHand);
    float GetZ();
    float GetTwist();
    float GetThrottle();
    float GetRawAxis(UINT32);
    bool GetTrigger(JoystickHand = kRightHand); //button 1
    bool GetTop(JoystickHand = kRightHand); //button 2
    bool GetBumper(JoystickHand = kRightHand); //just return joy->GetBumper() straight
    bool GetRawButton(UINT32);
};

#endif
