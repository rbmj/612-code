#ifndef DRIVE_PID_H_INC
#define DRIVE_PID_H_INC

#include <RobotDrive.h>
#include <PIDOutput.h>
#include "ports.h"

class drive_pid : public PIDOutput {
public:
    enum drive_pid_side_t {
        DRIVE_PID_LEFT,
        DRIVE_PID_RIGHT
    };
    drive_pid(drive_jaguar&, drive_jaguar&, drive_pid_side_t);
    void PIDWrite(float);
private:
    drive_jaguar* jag1;
    drive_jaguar* jag2;
    drive_pid_side_t side;
};

#endif
