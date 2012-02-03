/* vision/vision_processing.cpp
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
 * Entry points for vision processing: distance and angle from image
 */
 
/*
 * Calculated angles are relative to perpendicular.
 */

#include <ColorImage.h>
#include "ports.h"

ColorImage* vision_processing::get_image() {
    if(camera.IsFreshImage()) {
        camera.GetImage(old_image);
    } 
    else {
        return get_old_image();
    }
}

ColorImage* vision_processing::get_old_image() {
    return old_image;
}

double vision_processing::get_distance() {
    return get_distance_from_image(get_image());
}

double vision_processing::get_distance_from_image(ColorImage* image) {
    // TODO make it do stuff
}

double vision_processing::get_angle() {
    return get_angle_from_image(get_image());
}

double vision_processing::get_angle_from_image(ColorImage* image) {
    // TODO make it do stuff
}
