/* states/state_shooting.cpp
 *
 * Copyright (c) 2011, 2012 Chantilly Robotics <chantilly612@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * shooting state
 */
 
#include <vector>
#include "state_shooting.h"
#include "state_driving.h"
#include "../612.h"
#include "../vision/vision_processing.h"
#include "../ports.h"
#include "../visionalg.h"
#include "../vision_alt.h"
#include "../state_tracker.h"
#include "../vision_thread.h"
#include "../visionalg.h"

void aim();
void select();
void shoot();
static void change_state();
void print_targets(target*);

state_tracker shooting_substate(shooting_substate_selection);
target* primary_target = NULL;

void state_shooting() {
    // disable motor safety check to stop wasting netconsole space
    drive.SetSafetyEnabled(false);
    if(shooting_substate.get_state() == shooting_substate_selection) {
        select();
    }
    else if(shooting_substate.get_state() == shooting_substate_aiming) {
        aim();
    }
    else if(shooting_substate.get_state() == shooting_substate_shooting) {
        shoot();
    }
    if(!gunner_joystick.GetRawButton(SHOOT)) {
        change_state();
    }
}

void select() {
    primary_target = ascertain_primary_target();
    print_targets(primary_target);
    if(primary_target == NULL) {
        change_state();
        return;
    }
	shooter_turret.align(*primary_target);
	shooting_substate.set_state(shooting_substate_aiming);
}

void aim() {
    shooter_turret.enable();
    // TODO better tolerance detection (right now i think it detects distance from edge of image... not sure)
    if(std::abs(primary_target->x_offset()) < TARGET_X_TOLERANCE) // x_offset gives pixels
    {
		shooting_substate.set_state(shooting_substate_shooting);
	}
}

void shoot() {
	if(shooter_turret.Shooter().at_setpoint())
    {
		rollers.set_direction(roller_t::UP);
    }
    else
    {
		rollers.set_direction(roller_t::OFF);
    }
}

void change_state() {
    // TODO Find way To Center Turret While In Drive State
    shooter_turret.disable();
    shooting_substate.set_state(shooting_substate_selection);
    rollers.set_direction(roller_t::OFF);
    global_state.set_state(STATE_DRIVING);
    drive.SetSafetyEnabled(true);
}

void print_targets(target* primary_target) {
    vision_targets& ts = get_targets();
    std::vector<target> t_vec = ts.targets();
#ifdef VISION_ALT_ADHOC
    for(unsigned i = 0; i < numtargets && t_vec[i].valid(); i++) {
        std::printf("target %d - dx: %f, h: %f\n",i,t_vec[i].distance(),t_vec[i].height());
    }
#else
    if (ts.top().valid()) {
        std::printf("target top - dx: %f, h: %f\n",ts.top().distance(),ts.top().height());
    }
    else if (ts.left().valid()) {
        std::printf("target left - dx: %f, h: %f\n",ts.left().distance(),ts.left().height());
    }
    else if (ts.right().valid()) {
        std::printf("target right - dx: %f, h: %f\n",ts.right().distance(),ts.right().height());
    }
    else if (ts.bottom().valid()) {
        std::printf("target bottom - dx: %f, h: %f\n",ts.bottom().distance(),ts.bottom().height());
    }
#endif
    std::printf("chosen target - dx: %f, h: %f\n",primary_target->distance(),primary_target->height());
}
