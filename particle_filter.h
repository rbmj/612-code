#ifndef PARTICLE_FILTER_H_INC
#define PARTICLE_FILTER_H_INC

#include <nivision.h>
#include <Vision2009/VisionAPI.h>

class particle_filter {
private:
    Image * img;
public:
    particle_filter(Image * i) : img(i) {}
    bool operator()(const ParticleAnalysisReport&) const;
};


#endif
