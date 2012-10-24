#ifndef DRIVE_PID_H_INC
#define DRIVE_PID_H_INC

#include "612.h"
#include <RobotDrive.h>
#include <PIDOutput.h>
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif

class drive_pid : public PIDOutput {
public:
    enum drive_pid_side_t {
        SIDE_LEFT,
        SIDE_RIGHT
    };
    drive_pid(SpeedController&, SpeedController&, drive_pid_side_t, bool);
    void PIDWrite(float);
private:
    SPEEDCONTROLLER* jag1;
    SPEEDCONTROLLER* jag2;
    drive_pid_side_t side;
    bool reverse;
};

#endif
