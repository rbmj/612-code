#ifndef SHIFTER_H_INC
#define SHIFTER_H_INC

#include <Servo.h>

class shifter {
public:
    enum GEAR {
        LOW,
        HIGH,
        NEUTRAL
    };
    shifter(Servo&, Servo&);
    void set(GEAR);
private:
    shifter() {}
    void update();
    Servo * shift1;
    Servo * shift2;
    GEAR cur_gear;
    static void update_helper(void*);
};

#endif
