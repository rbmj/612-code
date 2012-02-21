#ifndef TURNTABLE_H_INC
#define TURNTABLE_H_INC

#include <Jaguar.h>
#include <DigitalInput.h>

class turntable {
public:
    enum cur_side {
        UNSURE,
        LEFT,
        RIGHT,
        CENTER
    };
    turntable(Jaguar&, DigitalInput&, DigitalInput&, DigitalInput&);
    ~turntable();
    void new_offset(int, int);
    void center();
    void enable();
    void disable();
    void manual_control(float);
private:
    Jaguar * jag;
    DigitalInput * left;
    DigitalInput * mid;
    DigitalInput * right;
    float power;
    bool centering;
    bool enabled;
    cur_side pos;
    void update();
    static void update_help(void*);
};

#endif
