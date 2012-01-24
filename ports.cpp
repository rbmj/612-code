/* ports.cpp
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
 * This file contains tables for the electrical layout and setup of the
 * electrical components.  Keep the formatting in this file very easy to read
 * such that it is easy to maintain and quickly check if it is in sync with
 * the actual state of affairs on the electrical board!
 */

#include <RobotDrive.h>
#include <Jaguar.h>
#include <Joystick.h>
#include <Encoder.h>
#include <Ultrasonic.h>
#include <DigitalInput.h>
#include <Relay.h>
#include "ports.h"

//just define & initialize all of the consts in ports.h

//PORTS TABLE

//PWMs                                  SLOT   PORT
Jaguar right_front_jag                  ( 4,     1 );
Jaguar right_rear_jag                   ( 4,     2 );
Jaguar left_front_jag                   ( 4,     3 );
Jaguar left_rear_jag                    ( 4,     4 );
Jaguar right_launcher_jag               ( 4,     5 );
Jaguar left_launcher_jag                ( 4,     6 );
Jaguar turret_XY_control_jag            ( 4,     7 );
Jaguar turret_Z_control_jag             ( 4,     8 );

//NOTE: Sica wants to use pots (potentiometers) for the angles, as those are
//absolute and don't need a zero switch.  If we do that they're seen as
//AnalogChannel s.  We'll want to write a quick wrapper class so we can get
//an angular offset, but we'll cross that bridge when we come to it.

//note: for two channel encoders, we need to specify a slot for both ports
//DIOs                                  SLOT   PORT
Encoder right_drive                     ( 4,     1,
                                          4,     2 );
Encoder left_drive                      ( 4,     3,
                                          4,     4 );
Encoder launcher_wheel                  ( 4,     5,
                                          4,     6 );
Encoder launch_angle                    ( 4,     7,
                                          4,     8 );

DigitalInput launch_angle_limit_switch  ( 6,     1 );
Ultrasonic front_sensor                 ( 6,     2,
                                          6,     3 );
// Servo servo_shift                       ( ?,     ? ); configuration TBD
                                          
//note: since we rely on the default value of kInches for the 5th arg
//we should use Ultrasonic::GetRangeInches().

//AIOs                                  SLOT   PORT

//Relays                                SLOT   PORT
Relay roller_spike_1                    ( 4,     1 );
Relay roller_spike_2                    ( 4,     2 );

//USBs (on driver station)                     PORT
Joystick left_joystick                         ( 1 );
Joystick right_joystick                        ( 2 );
Joystick gunner_joystick                       ( 3 );

//initialization of virtual devices:

//RobotDrive
RobotDrive drive (
    left_front_motor.jag,  //KEEP THIS ORDER!  IT'S IMPORTANT!
    left_rear_motor.jag,   //LF, LR, RF, RR -- see documentation
    right_front_motor.jag, //for more details
    right_rear_motor.jag
);

//drive_jaguar                           JAGUAR&                 TYPE               REVERSE
drive_jaguar left_front_motor =     { left_front_jag,  RobotDrive::kFrontLeftMotor,  true };
drive_jaguar left_rear_motor =      { left_rear_jag,   RobotDrive::kRearLeftMotor,   true };
drive_jaguar right_front_motor =    { right_front_jag, RobotDrive::kFrontRightMotor, true };
drive_jaguar right_rear_motor =     { right_rear_jag,  RobotDrive::kRearRightMotor,  true };
