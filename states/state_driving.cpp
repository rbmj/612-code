#ifndef STATE_DRIVING_H_INCLUDED
#define STATE_DRIVING_H_INCLUDED

#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

void state_driving(){
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
       
}


#endif // STATE_DRIVING_H_INCLUDED
