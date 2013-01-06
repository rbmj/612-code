#include "particle_filter.h"
#include "ranges.h"
#include "visionalg.h"
#include "612.h"
#include <nivision.h>
#include <Vision2009/VisionAPI.h>

//return true if particle should be removed
bool particle_filter::operator()(const ParticleAnalysisReport& r) const {
    //get minimum particle size
    int min_size = PARTICLE_AREA_MIN;
#ifdef RESOLUTION_320_240
    min_size /= 4;
#endif
#ifdef RESOLUTION_160_120
    min_size /= 4;
#endif
    if (r.particleArea < (double)min_size) {
        return true;
    }
    //change numxdivs/numydivs to make more/less precise
    const int numxdivs = 5;
    const int numydivs = 5;
    const int numpoints = (numxdivs * numydivs);
    Point p;
    int value = 0;
    int tot_filled_in = 0;
    p.x = r.boundingRect.left;
    for (int i = 0; i < numxdivs; i++) {
        p.x += r.boundingRect.width / (numxdivs + 2);
        p.y = r.boundingRect.top;
        for (int j = 0; j < numydivs; j++) {
            p.y += r.boundingRect.height / (numydivs + 2);
            imaqGetPixel(img, p, (PixelValue*)&value);
            if (value) {
                //filled in - change denom on next line to make more/less strict
                if (++tot_filled_in > numpoints / 4) {
                    //too many!
                    return true;
                }
            }
        }
    }
    return false;
}
