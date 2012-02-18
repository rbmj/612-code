#ifndef PID_CONTROLLER_H_INC
#define PID_CONTROLLER_H_INC

#include <PIDController.h>
#include <PIDSource.h>
#include <PIDOutput.h>

#include "612.h"

/* pid_controller will either be a SendablePIDController for debugging or a
 * regular PIDController for release.  The idea is that once competition time
 * comes we can remove this code and replace all of them with regular
 * PIDController objects.
 */

class pid_controller : public PIDController {
private:
    PIDController * pidobj;
#if DEBUG_612
    char * key;
#endif
public:
    pid_controller(float, float, float, PIDSource*, PIDOutput*, float = 0.05);
    ~pid_controller();
    
    //delegate everything
    float Get() { return pidobj->Get(); }
    void SetContinuous(bool c = true) { return pidobj->SetContinuous(c); }
    void SetInputRange(float min, float max) { return pidobj->SetInputRange(min, max); }
    void SetOutputRange(float min, float max) { return pidobj->SetOutputRange(min, max); }
    void SetPID(float p, float i, float d) { return pidobj->SetPID(p, i, d); }
    float GetP() { return pidobj->GetP(); }
    float GetI() { return pidobj->GetI(); }
    float GetD() { return pidobj->GetD(); }
    void SetSetpoint(float s) { return pidobj->SetSetpoint(s); }
    float GetSetpoint() { return pidobj->GetSetpoint(); }
    float GetError() { return pidobj->GetError(); }
    void SetTolerance(float p) { return pidobj->SetTolerance(p); }
    bool OnTarget() { return pidobj->OnTarget(); }
    void Enable() { return pidobj->Enable(); }
    void Disable() { return pidobj->Disable(); }
    bool IsEnabled() { return pidobj->IsEnabled(); }
    void Reset() { return pidobj->Reset(); }
};

#endif
