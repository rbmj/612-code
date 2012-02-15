/* vision_alt.h
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
 * An alternative implementation of image processing
 */

#ifndef VISION_ALT_H_INC
#define VISION_ALT_H_INC

#include <Vision/ColorImage.h>
#include <Vision/HSLImage.h>
#include <Vision2009/VisionAPI.h>

#include <vector>

#include "612.h"

//make our lives easier
typedef std::vector<ParticleAnalysisReport> report_vector;

class target {
private:
#ifdef VISION_ALT_HEURISTIC
    target() {} //make private
#endif
    double m_height; //should be constant
    double m_distance;
    int m_x_offset;
    bool m_valid;
    
    void update_data_with_report(const ParticleAnalysisReport&);
    
    static HSLImage image;
    static void id_and_process(report_vector * vec);
    static void three_target_arr_alignx(const ParticleAnalysisReport&,
            const ParticleAnalysisReport&, const ParticleAnalysisReport&);
    static void three_target_arr_aligny(const ParticleAnalysisReport&,
            const ParticleAnalysisReport&, const ParticleAnalysisReport&);
    //
public:
#ifdef VISION_ALT_ADHOC
    target() : m_valid(false) {}
#endif
    target(double h) : m_height(h) {} //feet
    bool valid() const { return m_valid; }
    int x_offset() const { return m_x_offset; } //pixels
    double distance() const { return m_distance; } //feet
    double height() const { return m_height; } //feet
    
    static void update_target_arr(void* = NULL);
};

#ifdef VISION_ALT_HEURISTIC

extern target bottom_basket;
extern target midleft_basket;
extern target midright_basket;
extern target top_basket;

#elif defined VISION_ALT_ADHOC

extern target target_arr[];
extern const unsigned numtargets;

#else
#error No valid VISION_ALT mode specified in 612.h
#endif

#endif
