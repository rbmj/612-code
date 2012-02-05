/* main.h
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
 * Define robot_class, which inherits from IterativeRobot and is used
 * to provide the WPILib Framework with the code for the robot.
 */

#ifndef INC_MAIN_H_INC
#define INC_MAIN_H_INC

#include <IterativeRobot.h>
#include <RobotDrive.h>

class robot_class : public IterativeRobot {
public:
    //ctor
    robot_class();

    //Virtual Overrides:
    //init
    void RobotInit();
    void DisabledInit();
    void AutonomousInit();
    void TeleopInit();
    //periodics
    void DisabledPeriodic();
    void AutonomousPeriodic();
    void TeleopPeriodic();
    //continuous
    void DisabledContinuous();
    void AutonomousContinuous();
    void TeleopContinuous();

    //added methods
    void update_sensors();
};

#endif
