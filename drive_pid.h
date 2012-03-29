#ifndef DRIVE_PID_H_INC
#define DRIVE_PID_H_INC

#include <RobotDrive.h>
#include <PIDOutput.h>
#include <Jaguar.h>

class drive_pid : public PIDOutput {
public:
    enum drive_pid_side_t {
        SIDE_LEFT,
        SIDE_RIGHT
    };
    drive_pid(Jaguar&, Jaguar&, drive_pid_side_t, bool);
    void PIDWrite(float);
private:
    Jaguar* jag1;
    Jaguar* jag2;
    drive_pid_side_t side;
    bool reverse;
};

#endif
