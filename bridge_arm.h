#ifndef BRIDGE_ARM_H_INC
#define BRIDGE_ARM_H_INC

#include <Relay.h>
#include <DigitalInput.h>

class bridge_arm_t {
public:
    enum direction {
        UP,
        DOWN,
        NEUTRAL
    };
    bridge_arm_t(Relay&, DigitalInput&);
    ~bridge_arm_t();
    void set_direction(direction);
    direction get_direction();
private:
    Relay * spike;
    DigitalInput * limit;
    direction dir;
    void update();
    static void update_help(void*);
};

#endif
