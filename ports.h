#ifndef INC_PORTS_H_INC
#define INC_PORTS_H_INC

#include <RobotDrive.h>
#include <Jaguar.h>
#include <Joystick.h>

struct drive_jaguar {
    Jaguar& jag;
    RobotDrive::MotorType type;
    bool reverse;
};


//PWMs
extern drive_jaguar left_front_motor;
extern drive_jaguar right_front_motor;
extern drive_jaguar left_rear_motor;
extern drive_jaguar right_rear_motor;

//DIOs

//AIOs

//Relays

//USBs (on driver station)
extern Joystick left_joystick;
extern Joystick right_joystick;

#endif
