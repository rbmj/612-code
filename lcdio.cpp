#include "lcdio.h"
#include "update.h"
#include "ports.h"
#include "vision_alt.h"
#include "trajectory.h"
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
    update_set_angle(rad2deg(shooter_turret.Winch().get_set_angle()));
    update_cur_angle(rad2deg(shooter_turret.Winch().get_cur_angle()));
    update_set_speed(shooter_turret.Shooter().get_set_freq());
    update_cur_speed(shooter_turret.Shooter().get_cur_freq());
#ifdef VISION_ALT_HEURISTIC
    //targets
    update_target_ok(top_basket.valid(), midleft_basket.valid(), midright_basket.valid(), bottom_basket.valid());
    //calculate average dx
    double acc = 0.0;
    int div = 0;
    if (top_basket.valid()) {
        acc += top_basket.distance();
        div++;
    }
    if (midleft_basket.valid()) {
        acc += midleft_basket.distance();
        div++;
    }
    if (midright_basket.valid()) {
        acc += midright_basket.distance();
        div++;
    }
    if (bottom_basket.valid()) {
        acc += bottom_basket.distance();
        div++;
    }
    if (div) {
        update_target_dx(acc/div);
    }
    else {
        update_target_dx_no_targets();
    }
#elif defined VISION_ALT_ADHOC
    //this could be better - just let the array corrospond to the targets
    //this may not be clear to the driver
    update_target_ok(target_arr[0].valid(), target_arr[1].valid(), target_arr[2].valid(), target_arr[3].valid());
    double acc = 0.0;
    int div = 0;
    for (int i = 0; i < numtargets; i++) {
        if (target_arr[i].valid()) {
            acc += target_arr[i].distance();
            div++;
        }
    }
    if (div) {
        update_target_dx(acc/div);
    }
    else {
        update_target_dx_no_targets();
    }
#endif
    direction dir;
    roller_t::direction d = rollers.get_direction();
    switch (d) {
    case roller_t::UP:
        dir = POSITIVE;
        break;
    case roller_t::DOWN:
        dir = NEGATIVE;
        break;
    default:
        dir = NEUTRAL;
        break;
    }
    update_roller(dir);
    bridge_arm_t::direction d2 = bridge_arm.get_direction();
    switch (d2) {
    case bridge_arm_t::UP:
        dir = POSITIVE;
        break;
    case bridge_arm_t::DOWN:
        dir = NEGATIVE;
        break;
    default:
        dir = NEUTRAL;
        break;
    }
    update_bridge(dir);
    float f = shooter_turret.Susan().get_power();
    if (f > 0.0) {
        dir = POSITIVE;
    }
    else if (f < 0.0) {
        dir = NEGATIVE;
    }
    else {
        dir = NEUTRAL;
    }
    update_turret(dir);
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

void lcdio::update_target_dx_no_targets() {
    lcdp->Printf(DriverStationLCD::kUser_Line4, 12, "NONE");
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

//force instantiation of lcdio:
const lcdio& lcd_reference_force_ = lcd();
