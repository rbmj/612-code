#ifndef OVERRIDE_CONTROLS_H_INCLUDED
#define OVERRIDE_CONTROLS_H_INCLUDED

#include "612.h"
#include "vision/vision_processing.h"
#include "ports.h"
#include "visionalg.h"

//TEMPORARY: GUNNER LAUNCHER WHEEL CONTROLS
void override_controls() {
    //Turret rotation controlled by gunner joystick during drive state only. Must press button 1
    if (gunner_joystick.GetRawButton(1)) {
            turret_rotation_jag.Set(-gunner_joystick.GetX());
    }

    //Bridge
    if(gunner_joystick.GetRawButton(2)){//up
        if(bridge_arm_switch.Get()!=1){//limit switch not pressed
            printf("bridge DOWN");
            bridge_arm_spike.Set(Relay::kForward);
        }
    }
    else if(gunner_joystick.GetRawButton(3)){//down
        printf("bridge UP");
        bridge_arm_spike.Set(Relay::kReverse);
    }
    else {
            bridge_arm_spike.Set(Relay::kOff);
    }

    //Winch
    if(gunner_joystick.GetRawButton(6)) {
            turret_winch_jag.Set(-0.2);
    }
    else if(gunner_joystick.GetRawButton(7)) {
        turret_winch_jag.Set(0.2);
    }
    else {
        turret_winch_jag.Set(0.0);
    }

    // pot
    if(gunner_joystick.GetRawButton(8)) {
        printf("pot voltage: %f\n", launch_angle_pot.GetVoltage());
    }
    if(gunner_joystick.GetRawButton(6)) {
        turret_winch_jag.Set(-0.2);
    }
    else if(gunner_joystick.GetRawButton(7)) {
        turret_winch_jag.Set(0.2);
    }
    else {
        turret_winch_jag.Set(0.0);
    }
    if(gunner_joystick.GetRawButton(9)) {
        printf("pot voltage: %f\n", launch_angle_pot.GetVoltage());
    }

    //TEMPORARY: GUNNER LAUNCHER WHEEL CONTROLS
    if (gunner_joystick.GetRawButton(4)) {
        left_launcher_jag.Set(1.0);
        right_launcher_jag.Set(1.0);
        std::printf("Launcher Wheel Speed: %f\n", 1/launcher_wheel.GetPeriod());
    }
    else {
        left_launcher_jag.Set(-0.05);
        right_launcher_jag.Set(-0.05);
    }

    //MANUAL ROLLER CONTROL
    if (gunner_joystick.GetRawButton(11)) {
        //rollers up
        rollers.set_direction(roller_t::UP);
    }
    else if (gunner_joystick.GetRawButton(10)) {
        //rollers down
        rollers.set_direction(roller_t::DOWN);
    }
    else {
        rollers.set_direction(roller_t::OFF);
    }
}


#endif // OVERRIDE_CONTROLS_H_INCLUDED
