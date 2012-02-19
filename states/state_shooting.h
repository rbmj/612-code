#ifndef STATE_SHOOTING_H_INCLUDED
#define STATE_SHOOTING_H_INCLUDED

#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

void shooting_auto(){
if (global_state.get_state() == STATE_SHOOTING) {
        // disable motor safety check to stop wasting netconsole space
        drive.SetSafetyEnabled(false);
        vision_processing::update();
        vector<double> target_degrees = vision_processing::get_degrees();
        vector<double> target_distances = vision_processing::get_distance();
        printf("Number of targets detected: %d\n", target_degrees.size());
        if(target_degrees.size() >= 1) {
            printf("Angle (degrees) of camera: %f\n", target_degrees[0]);
        }
        else {
            printf("No target detected\n");
        }
        if(target_distances.size() >= 1) {
            printf("Distance of target:       %f\n", target_distances[0]);
        }
        if(!left_joystick.GetRawButton(3)) {
            global_state.set_state(STATE_DRIVING);
            drive.SetSafetyEnabled(true);
        }
     }
}



#endif // STATE_SHOOTING_H_INCLUDED
