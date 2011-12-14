/* main.cpp
 * 
*/

#include "main.h"
#include "ports.h"

//constructor - initialize drive
robot_class::robot_class() :
    drive (     left_front_motor.jag,
                left_rear_motor.jag,
                right_front_motor.jag,
                right_rear_motor.jag
    )
{
    GetWatchdog().SetEnabled(false); //we don't want Watchdog
    //now set the necessary inversions
    drive.SetInvertedMotor(left_front_motor.type,  left_front_motor.reverse);
    drive.SetInvertedMotor(left_rear_motor.type,   left_rear_motor.reverse);
    drive.SetInvertedMotor(right_front_motor.type, right_front_motor.reverse);
    drive.SetInvertedMotor(right_rear_motor.type,  right_rear_motor.reverse);
}

void robot_class::RobotInit() {
    //do nothing
}

void robot_class::DisabledInit() {
    //do nothing
}

void robot_class::AutonomousInit() {
    //do nothing
}

void robot_class::TeleopInit() {
    //do nothing
}

void robot_class::DisabledPeriodic() {
    //do nothing
}

void robot_class::AutonomousPeriodic() {
    //do nothing
}

void robot_class::TeleopPeriodic() {
    //do nothing
}

void robot_class::DisabledContinuous() {
    //do nothing
}

void robot_class::AutonomousContinuous() {
    //do nothing
}

void robot_class::TeleopContinuous() {
    //actually do something!! :D
    drive.TankDrive(left_joystick, right_joystick);
    Wait(0.005);
}

//the following macro tells the library that we want to generate code
//for our class robot_class
START_ROBOT_CLASS(robot_class);
