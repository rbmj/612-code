/* roller.cpp
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
 * Implement roller control system.
 */
 
#include "roller.h"
#include "update.h"

#include <Relay.h>

roller_t::roller_t(Relay& a, Relay& b) : spike1(&a), spike2(&b) {
    m_direction = (unsigned char) OFF;
    registry().register_func(update_helper, (void*)this);
}

roller_t::~roller_t() {
    registry().unregister_func(update_helper, (void*)this);
}

void roller_t::update_helper(void * obj) {
    ((roller_t*)obj)->update();
}

void roller_t::update() {
    if (m_direction == (unsigned char) DOWN) {
        spike1->Set(Relay::kForward);
        spike2->Set(Relay::kForward);
    }
    else if (m_direction == (unsigned char) UP) {
        spike1->Set(Relay::kReverse);
        spike2->Set(Relay::kReverse);
    }
    else {
        //for saftey, default to OFF
        spike1->Set(Relay::kOff);
        spike2->Set(Relay::kOff);
    }
}

void roller_t::set_direction(direction d) {
    m_direction = (unsigned char) d;
}
