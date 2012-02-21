#ifndef LCDIO_H_INC
#define LCDIO_H_INC

#include <DriverStationLCD.h>

class lcdio {
public:
    enum direction {
        POSITIVE,
        NEGATIVE,
        NEUTRAL
    };
    lcdio();
    ~lcdio();
private:
    DriverStationLCD * lcdp;
    static void update_help(void*);
    void update();
    void update_set_speed(float);
    void update_cur_speed(float);
    void update_set_angle(float);
    void update_cur_angle(float);
    void update_target_dx(float);
    void update_target_ok(bool, bool, bool, bool);
    void update_roller(direction);
    void update_bridge(direction);
    void update_turret(direction);
    static const char * direction2str(direction, const char*, const char*, const char*);
};

lcdio& lcd();

#endif
