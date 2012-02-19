#ifndef TURNTABLE_H_INC
#define TURNTABLE_H_INC

#include <Jaguar.h>
#include <DigitalInput.h>

class turntable {
public:
    turntable(Jaguar&, DigitalInput&, DigitalInput&, DigitalInput&);
    void new_offset(int, int);
    void center();
    void enable();
    void disable();
private:
    Jaguar * jag;
    DigitalInput * left;
    DigitalInput * mid;
    DigitalInput * right;
    float power;
    bool centering;
    bool enabled;
    void update();
    static void update_help(void*);
};

#endif
