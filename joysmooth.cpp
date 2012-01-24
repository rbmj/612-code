#ifdef JOYSMOOTH_H_INC
#define JOYSMOOTH_H_INC

#include "update.h"

void register_callback(void*);
void update_callback_joysmooth(void*);

joysmooth(GenericHID* ghid) : joy(ghid) {
    register_callback((void*)this);
}

joysmooth(GenericHID& ghid) : joy(&ghid) {
    register_callback((void*)this);
}

~joysmooth() {
    registry.unregister_func(update_callback, (void*)this);
}

joysmooth::void update() {
    for (int d=0;d<NUMBUTTONS;d++)
    {
        buttons[d][0]=buttons[d][1];
        buttons[d][1]=joy->GetRawButton(d);
    }
    for(int d=0;d<NUMAXES;d++)
    {
        axes[d]=joy->GetRawAxis(d);
    }
}

joysmooth::float GetX(GenericHID::JoystickHand h) {
   return axes[X_AXIS];
}

joysmooth::float GetY(GenericHID::JoystickHand h) {
   return axes[Y_AXIS];
}

joysmooth::float GetZ(){
    return axes[Z_AXIS];
}

joysmooth::float GetTwist(){
    return axis[TWIST_AXIS];
}

joysmooth::float GetThrottle(){
    return axes[THROTTLE_AXIS];
}

joysmooth::float GetRawAxis(uninteger axisId){
    return axes[axisId];
}

joysmooth::bool GetTop(){
    return GetRawButton(2);
}

joysmooth::bool GetBumper(){
    return joy->GetBumper();
}

joysmooth::bool GetRawButton(uinteger btnId){
    for(int i=0;i<HOLDBACK;i++) {
        if(!buttons[btnId][i]) {
            return false;
        }
    }
    return true;
}

void register_callback(void* thisPtr) {
    registry.register_func(update_callback_joysmooth, thisPtr);
}

void update_callback_joysmooth(void* thisPtr) {
    ((joysmooth*)thisPtr)->update();
}

#endif
