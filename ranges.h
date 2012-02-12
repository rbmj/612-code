/* ranges.h
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
 * const range_t values for image threshold ranges and other ranges
 */

#ifndef INC_RANGES_H
#define INC_RANGES_H

#include "612.h"

// ==== HSV ====

const range_t HSV_HMIN =  72;
const range_t HSV_HMAX = 129;

const range_t HSV_SMIN = 110;
const range_t HSV_SMAX = 255;

const range_t HSV_VMIN = 193;
const range_t HSV_VMAX = 255;

// ==== HSI ====

const range_t HSI_HMIN =  72;
const range_t HSI_HMAX = 129;

const range_t HSI_SMIN =  30;
const range_t HSI_SMAX = 255;

const range_t HSI_IMIN = 174;
const range_t HSI_IMAX = 241;

// ==== HSL ====

const range_t HSL_HMIN =  83;//080;
const range_t HSL_HMAX = 129;//116;

const range_t HSL_SMIN =  50;//075;
const range_t HSL_SMAX = 255;//255;

const range_t HSL_LMIN =  92;//131;
const range_t HSL_LMAX = 246;//199;

enum COLOR_MODE_T {
    HSL,
    HSI,
    HSV
};

//set these to change search color
const COLOR_MODE_T COLOR_MODE = HSL;

//configure the particle
const int PARTICLE_AREA_MIN = 100;
const int PARTICLE_AREA_MAX = 50000;

#endif // INC_RANGES_H
