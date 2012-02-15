/* Turret.h
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

#ifndef INC_Turret_H
#define INC_Turret_H

#include <Jaguar.h>
#include <PIDController.h>
#include <Counter.h>
#include "612.h"
#include "launch_counter.h"
#include "vision_alt.h"
#include "two_jags.h"

class turret {
public:
    enum DIRECTION {
        LEFT = 0,
        RIGHT = 1
    }
	turret(Jaguar&, Jaguar&, Jaguar&, Jaguar&, Counter&);
    ~turret();
	void align(target&);
    void enable();
    void disable();
    void turn(DIRECTION);
private:
    turret() {}
    target * cur_target;

    Jaguar * rotation_jag;
    Jaguar * winch_jag;

    PIDController * launch_control;
    two_jags * launcher_jags;
    launch_counter * launch_speed;
#if DEBUG_612
    const char * key;
#endif
};


#endif
