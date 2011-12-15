#include "ports.h"

//just define & initialize all of the consts in ports.h


//PWMs                       SLOT   PORT
Jaguar left_rear_jag        ( 4,     2 );
Jaguar right_rear_jag       ( 4,     3 );
Jaguar right_front_jag      ( 4,     4 );
Jaguar left_front_jag       ( 4,     5 );

//DIOs

//AIOs

//Relays

//USBs (on driver station)         PORT
Joystick            left_joystick ( 1  );
Joystick            right_joystick( 2  );

//drive_jaguar                           JAGUAR&                 TYPE               REVERSE
drive_jaguar left_front_motor =     { left_front_jag,  RobotDrive::kFrontLeftMotor,  false };
drive_jaguar left_rear_motor =      { left_rear_jag,   RobotDrive::kRearLeftMotor,   false };
drive_jaguar right_front_motor =    { right_front_jag, RobotDrive::kFrontRightMotor, false };
drive_jaguar right_rear_motor =     { right_rear_jag,  RobotDrive::kRearRightMotor,  false };
