/* override_controls.h
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
 * Controls for overriding automatic shooting
 */

#ifndef INC_OVERRIDE_CONTROLS_H_INC
#define INC_OVERRIDE_CONTROLS_H_INC

#include "buttons.h"

extern const int TURRET_ROTATION;
extern const int SHOOT_SPIN;
extern const int ACQUIRE_TARGET;
extern const int SHOOT_KEY;
extern const int SHOOT_FENDER;
extern const int ROLLERS_DOWN;
extern const int ROLLERS_UP;
extern const int WINCH_OVERRIDE;
extern const int SHOOTER_OVERRIDE;
extern const int BRIDGE_ARM_DOWN;
extern const int BRIDGE_ARM_UP;


void gunner_override_controls();

void bridge_arm_up();
void bridge_arm_down();
void bridge_arm_neutral();

void rollers_up();
void rollers_down();
void rollers_off();

void shoot_key();
void shoot_spin();
void shoot_fender();
void turret_acquire();

#endif
