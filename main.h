/* main.h
 * 
*/

#ifndef INC_MAIN_H_INC
#define INC_MAIN_H_INC

#include "ports.h"
#include <IterativeRobot.h>

class robot_class : public IterativeRobot {
private:
    RobotDrive drive;
public:
    robot_class();
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
};


#endif
