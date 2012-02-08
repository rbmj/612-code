/* visionalg.h
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
 * Provide vision processing algorithms.
 */

#ifndef VISIONALG_H_INC
#define VISIONALG_H_INC

double angle_offset(int, int, double); //offset, total, FOV

class aspect_ratio {
private:
    int x;
    int y;
public:
    int X() const { return x; }
    int Y() const { return y; }
    aspect_ratio(int, int);
};

class camera_fov {
private:
    double field_of_view;
    double field_of_view_x;
    double field_of_view_y;
    aspect_ratio ratio;
public:
    operator double() const { return field_of_view; }
    double X() const { return field_of_view_x; }
    double Y() const { return field_of_view_y; }
    const aspect_ratio& ASPECT() const { return ratio; }
    camera_fov(double, aspect_ratio);
};

const camera_fov& FOV();

/* call thusly:
 * 
 * FOV() - camera field of view along dia
 * FOV().X() - field of view along x axis
 * FOV().Y() - field of view along y axis
 * FOV().ASPECT().X() - x component of AR
 * FOV().ASPECT().Y() - y component of AR
 */
 
const aspect_ratio& RESOLUTION();

/* call thusly:
 * 
 * RESOLUTION().X() - x resolution
 * RESOLUTION().Y() - y resolution
 * 
 */
 


#endif
