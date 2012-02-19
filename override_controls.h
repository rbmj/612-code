#ifndef OVERRIDE_CONTROLS_H_INCLUDED
#define OVERRIDE_CONTROLS_H_INCLUDED

#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

//TEMPORARY: GUNNER LAUNCHER WHEEL CONTROLS
void shooting_manual() {
if (gunner_joystick.GetRawButton(7)) {
        left_launcher_jag.Set(1.0);
        right_launcher_jag.Set(1.0);
    }
    else {
        left_launcher_jag.Set(0.0);
        right_launcher_jag.Set(0.0);
    }
    if (global_state.get_state() != STATE_SHOOTING) {
        //MANUAL ROLLER CONTROL
        if (gunner_joystick.GetRawButton(11)) {
            //rollers up
            rollers.set_direction(roller_t::UP);
        }
        else if (gunner_joystick.GetRawButton(10)) {
            //rollers down
            rollers.set_direction(roller_t::DOWN);
        }
        else if (gunner_joystick.GetRawButton(9)) {
            rollers.set_direction(roller_t::OFF);
        }
        else {
            //auto belts
        }
    }
}


#endif // OVERRIDE_CONTROLS_H_INCLUDED
