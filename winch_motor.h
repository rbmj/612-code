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

#ifndef INC_WINCH_MOTOR_H
#define INC_WINCH_MOTOR_H

#include "612.h"
#include <SpeedController.h>
#ifdef USE_JAGUARPWM
#include <Jaguar.h>
#else
#include <CANJaguar.h>
#endif
#include <DigitalInput.h>
#include <PIDOutput.h>

class winch_motor : public PIDOutput {
private:
    SPEEDCONTROLLER* jag;
    DigitalInput* limit_switch;
public:
    winch_motor(SpeedController&, DigitalInput&);
    void PIDWrite(float);
};

#endif // INC_WINCH_MOTOR_H
