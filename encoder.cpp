#include "encoder.h"
#include "ports.h"
#include <RobotDrive.h>

//the gear reduction from the encoder shaft to the drive shafts is 38:60
//the wheels are about 8" in diameter.
//the encoders on those wheels are 250 ticks per revolution

//therefore distance = ticks * (250/(2pi)) * (38/60) * (8/2)
