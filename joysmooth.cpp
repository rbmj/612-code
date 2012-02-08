#ifdef JOYSMOOTH_H_INC
#define JOYSMOOTH_H_INC

#include <bitset>
#include <GenericHID.h>
#include "612.h"
#include "update.h"
#include "joysmooth.h"

//provide definition of constants:
const int joysmooth::HOLDBACK;
const int joysmooth::NUMBUTTONS;
const int joysmooth::NUMAXES;

void register_callback(void*);
void update_callback_joysmooth(void*);

joysmooth::joysmooth(GenericHID* ghid) : joy(ghid) {
    register_callback((void*)this);
}

joysmooth::joysmooth(GenericHID& ghid) : joy(&ghid) {
    register_callback((void*)this);
}

joysmooth::~joysmooth() {
    registry().unregister_func(update_callback, (void*)this);
}

void joysmooth::update() {
    for (int d = 0; d < NUMBUTTONS; d++) {
        for (int i = 1; i < HOLDBACK; i++) {
            buttons[d][i - 1] = buttons[d][i];
        }
        buttons[d][HOLDBACK - 1] = joy->GetRawButton(d);
    }
    for(int d = 0; d < NUMAXES; d++) {
        axes[d] = joy->GetRawAxis(d);
    }
}

float joysmooth::GetX(GenericHID::JoystickHand h) {
   return axes[X_AXIS];
}

float joysmooth::GetY(GenericHID::JoystickHand h) {
   return axes[Y_AXIS];
}

float joysmooth::GetZ(){
    return axes[Z_AXIS];
}

float joysmooth::GetTwist(){
    return axis[TWIST_AXIS];
}

float joysmooth::GetThrottle(){
    return axes[THROTTLE_AXIS];
}

float joysmooth::GetRawAxis(UINT32 axisId){
    return axes[axisId];
}

bool joysmooth::GetTop(){
    return GetRawButton(2);
}

bool joysmooth::GetBumper(){
    return joy->GetBumper();
}

bool joysmooth::GetRawButton(UINT32 btnId){
    for(int i = 0; i < HOLDBACK; i++) {
        if(!buttons[btnId][i]) {
            return false;
        }
    }
    return true;
}

void register_callback(void* thisPtr) {
    registry().register_func(update_callback_joysmooth, thisPtr);
}

void update_callback_joysmooth(void* thisPtr) {
    ((joysmooth*)thisPtr)->update();
}

#endif
