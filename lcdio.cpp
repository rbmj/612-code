#include "lcdio.h"
#include "update.h"
#include <DriverStationLCD.h>

/* Format like this: */
const char * line_fmt[] = {
    "Shoot:   [SET]  [CUR]",
    "  Speed- 00.00  00.00",
    "  Angle- 00.00  00.00",
    "Targets dx 00.0   -  ",
    "Roll: - Brdg: -  - - ",
    "Turr: -           -  ",
};
 
lcdio& lcd() {
    static lcdio obj;
    return obj;
}

lcdio::lcdio() {
    registry().register_func(lcdio::update_help, (void*)this);
    lcdp = DriverStationLCD::GetInstance();
    //put initial values:
    lcdp->PrintfLine(DriverStationLCD::kUser_Line1, line_fmt[0]);
    lcdp->PrintfLine(DriverStationLCD::kUser_Line2, line_fmt[1]);
    lcdp->PrintfLine(DriverStationLCD::kUser_Line3, line_fmt[2]);
    lcdp->PrintfLine(DriverStationLCD::kUser_Line4, line_fmt[3]);
    lcdp->PrintfLine(DriverStationLCD::kUser_Line5, line_fmt[4]);
    lcdp->PrintfLine(DriverStationLCD::kUser_Line6, line_fmt[5]);
}

lcdio::~lcdio() {
    registry().unregister_func(lcdio::update_help, (void*)this);
}

void lcdio::update() {
    lcdp->UpdateLCD();
}

void lcdio::update_help(void * obj) {
    ((lcdio*)obj)->update();
}

void lcdio::update_set_speed(float speed) {
    lcdp->Printf(DriverStationLCD::kUser_Line2, 10, "%02.2f", speed);
}

void lcdio::update_cur_speed(float speed) {
    lcdp->Printf(DriverStationLCD::kUser_Line2, 17, "%02.2f", speed);
}

void lcdio::update_set_angle(float angle) {
    lcdp->Printf(DriverStationLCD::kUser_Line3, 10, "%02.2f", angle);
}

void lcdio::update_cur_angle(float angle) {
    lcdp->Printf(DriverStationLCD::kUser_Line3, 17, "%02.2f", angle);
}

void lcdio::update_target_dx(float dist) {
    lcdp->Printf(DriverStationLCD::kUser_Line4, 12, "%02.1f", dist);
}

void lcdio::update_target_ok(bool top, bool left, bool right, bool bot) {
    lcdp->Printf(DriverStationLCD::kUser_Line4, 19, (top ? "+" : "-"));
    lcdp->Printf(DriverStationLCD::kUser_Line5, 18, (left ? "+" : "-"));
    lcdp->Printf(DriverStationLCD::kUser_Line5, 20, (right ? "+" : "-"));
    lcdp->Printf(DriverStationLCD::kUser_Line6, 19, (bot ? "+" : "-"));
}

const char * lcdio::direction2str(lcdio::direction dir, const char * pos, const char * neg, const char * zero) {
    switch (dir) {
    case lcdio::POSITIVE:
        return pos;
    case lcdio::NEGATIVE:
        return neg;
    default:
        return zero;
    }
}

void lcdio::update_roller(lcdio::direction dir) {
    lcdp->Printf(DriverStationLCD::kUser_Line5, 7, direction2str(dir, "^", "v", "-"));
}

void lcdio::update_bridge(lcdio::direction dir) { 
    lcdp->Printf(DriverStationLCD::kUser_Line5, 15, direction2str(dir, "^", "v", "-"));
}

void lcdio::update_turret(lcdio::direction dir) {
    lcdp->Printf(DriverStationLCD::kUser_Line6, 7, direction2str(dir, ">", "<", "|"));
}
