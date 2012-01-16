/* ports.h
 *
 * Copyright (c) 2011, 2012 Chantilly Robotics <chantilly612@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Provide extern definitions for all of the variables used for interacting
 * with the electrical components of the robot
 */

#ifndef INC_PORTS_H_INC
#define INC_PORTS_H_INC

#include <RobotDrive.h>
#include <Jaguar.h>
#include <Joystick.h>
#include "update.h"

//bundle a reference to a Jaguar and some constants to aid in setting up the
//robot's drivetrain.  Use this for jaguars that will be used in the RobotDrive
//drivetrain object.
struct drive_jaguar {
    Jaguar& jag; //reference to prevent multiple definition
    const RobotDrive::MotorType type;
    const bool reverse;
};

//actual definitions.  Note they all must be declared 'extern'!


//PWMs
extern drive_jaguar left_front_motor;
extern drive_jaguar right_front_motor;
extern drive_jaguar left_rear_motor;
extern drive_jaguar right_rear_motor;
extern Jaguar left_launcher_jag;
extern Jaguar right_launcher_jag;
extern Jaguar turret_XY_control_jag;
extern Jaguar turret_Z_control_jag;

//DIOs
extern Encoder right_drive;
extern Encoder left_drive;
extern Encoder launcher_wheel;
extern Encoder launch_angle;
extern Ultrasonic front_sensor;
extern DigitalInput launch_angle_limit_switch;

//AIOs

//Relays
extern Relay roller_spike_1;
extern Relay roller_spike_2;

//USBs (on driver station)
extern Joystick left_joystick;
extern Joystick right_joystick;
extern Joystick gunner_joystick;

//Virtual Devices
extern RobotDrive drive;
extern update_registry registry;

#endif
