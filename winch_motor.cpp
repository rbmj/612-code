/* winch_motor.h
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
 * Wrapper class for winch jag to incoporate limit switch
 */

#include "612.h"
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif
#include <DigitalInput.h>
#include "winch_motor.h"

winch_motor::winch_motor(SpeedController& j, DigitalInput& l) {
    jag = static_cast<SPEEDCONTROLLER*>(&j);
    limit_switch = &l;
}

void winch_motor::PIDWrite(float output) {
    printf("output: %f\n", output);
    if(output > 0) {
        if(limit_switch->Get() == 1) { // switch pressed, do not go down
            printf("winch switch PRESSED\n");
            output = -output;
        }
    }
    jag->PIDWrite(output);
}
