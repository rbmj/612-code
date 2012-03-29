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

state_tracker shooting_substate(state_selection);
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
    if(!gunner_joystick.GetRawButton(1)) {
		// TODO Find way To Center Turret While In Drive State
		shooter_turret.disable();
		shooting_substate.set_state(shooting_substate_selection);
		rollers.set_direction(roller_t::OFF);
        global_state.set_state(STATE_DRIVING);
        drive.SetSafetyEnabled(true);
    }
}

void select() {
    primary_target = ascertain_primary_target();
	shooter_turret.align(PrimaryTarget);
	shooting_substate.set_state(shooting_substate_aiming);
}

void aim() {
    shooter_turret.enable();
    // TODO better tolerance detection (right now i think it detects distance from edge of image... not sure)
    if(primary_target->x_offset() < TOLERANCE) // x_offset gives pixels
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
