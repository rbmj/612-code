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
 
#ifndef STATE_SHOOTING_H_INCLUDED
#define STATE_SHOOTING_H_INCLUDED

#include "../state_tracker.h"

const unsigned int TARGET_X_TOLERANCE = 15; // number of pixels target is allowed to be off center

enum SHOOTING_SUBSTATE {
    shooting_substate_selection,
    shooting_substate_aiming,
    shooting_substate_shooting
}

extern state_tracker shooting_substate;

void state_shooting();

#endif // STATE_SHOOTING_H_INCLUDED
