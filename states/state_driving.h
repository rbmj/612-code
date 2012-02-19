#ifndef STATE_DRIVING_H_INCLUDED
#define STATE_DRIVING_H_INCLUDED

#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

void shooting_driving(){
if (global_state.get_state() == STATE_DRIVING) {
        //Turret rotation controlled by gunner joystick during drive state only. Must press button 1
        if (gunner_joystick.GetRawButton(1)) {
            turret_rotation_jag.Set(gunner_joystick.GetX());
        }
        if (left_joystick.GetRawButton(1)) {
            //arcade drive
            drive.ArcadeDrive(left_joystick); //arcade drive on left joystick
        }
        else {
            //tank drive
            float left = left_joystick.GetY();
            float right = right_joystick.GetY();
            //explicitly state drive power is based on Y axis of that side joy
            drive.TankDrive(left, right);
        }
        if (right_joystick.GetRawButton(11) || left_joystick.GetRawButton(11)) {
            servo_shifter.set(shifter::HIGH);
            // set servo to high gear
        }
        else if (left_joystick.GetRawButton(10) || right_joystick.GetRawButton(10)) {
            servo_shifter.set(shifter::LOW);
            //Sets servo to low gear
        }
        if(left_joystick.GetRawButton(3)) {
            global_state.set_state(STATE_SHOOTING);
        }
        //Turret rotation controlled by gunner joystick during drive state only. Must press button 1
        if(gunner_joystick.GetRawButton(1){
            turret_rotation_jag.Set(gunner_joystick.GetX());
        }
        if(bridge_arm_switch.get()!=1){//limit switch not pressed
            bridge_arm_spike.set(kOn);
            if(gunner_joystick.GetRawButton(2){//up
               bridge_arm_spike.set(kForwardOnly);
            }
            if(gunner_joystick.GetRawButton(3){//down
               bridge_arm_spike.set(kReverseOnly);
            }
        }
        bridge_arm_spike.set(kOff);
    }
}


#endif // STATE_DRIVING_H_INCLUDED
