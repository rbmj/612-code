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
#include <Encoder.h>
#include <Ultrasonic.h>
#include <DigitalInput.h>
#include <Relay.h>
#include <Servo.h>
#include <AnalogChannel.h>
#include <Vision/AxisCamera.h>
#include "joysmooth.h"
#include "roller.h"
#include "shifter.h"

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
extern Jaguar turret_rotation_jag;
extern Jaguar turret_winch_jag;
extern Servo right_servo_shift;
extern Servo left_servo_shift;

//DIOs
extern Encoder right_drive;
extern Encoder left_drive;
extern Counter launcher_wheel;
extern Encoder launch_angle;
extern Ultrasonic front_ultrasonic;
extern DigitalInput launch_angle_switch;

//AIOs
extern AnalogChannel launch_angle_pot;

//Relays
extern Relay roller_spike_1;
extern Relay roller_spike_2;
extern Relay bridge_arm_spike;

//USBs (on driver station)
extern joysmooth left_joystick;
extern joysmooth right_joystick;
extern joysmooth gunner_joystick;

extern const char * cameraIP;

static inline AxisCamera& camera() {
    return AxisCamera::GetInstance(cameraIP);
}

//Virtual Devices
extern RobotDrive drive;
extern roller_t rollers;
extern shifter servo_shifter;
/* turret 
extern turret shooter_turret;
*/

#endif
