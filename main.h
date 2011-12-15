/* main.h
 *
*/

#ifndef INC_MAIN_H_INC
#define INC_MAIN_H_INC

#include <IterativeRobot.h>
#include <RobotDrive.h>

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

typedef UINT32 uinteger;


#endif
