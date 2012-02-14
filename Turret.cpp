#include "Turret.h"
#include "vision/vision_processing.h"
#include "ports.h"

const int Zero_Max = 4;
const int Zero_Min = -1;
const int Left = 0;
const int Right = 1;
const float Turn_Speed = 0.1;

void Turret::Align(int Target) {
    while(Direction(Target)==Left) {
        Jaguar turret_rotation_jag.Set(-1*Turn_Speed);
    }
    while(Direction(Target)==Right) {
        Jaguar turret_rotation_jag.Set(Turn_Speed);
    }
}

int Difference(int target) {
    int Picture_Center = ; //Picture Width/2
    int Target_Center_Mass_X = ;//Add code Here
    return (Picture_Center-Target_Center_Mass_X);
}

int Direction(int Target) {
    int Diff=Difference(Target);
    if((Diff<10)&&(Diff>-10)) {
        return 2;
    }
    else if (Diff>10) {
        return Left;
    }
    else {
        return Right;
    }
}

