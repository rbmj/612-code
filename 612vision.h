#ifndef VISION_H_INC_612
#define VISION_H_INC_612

#include <nivision.h>

class CImage {}
public:
    CImage();
    CImage(ImageType);
    CImage(const CImage&);
    ~CImage();
    int crop(Rect);
//pages 24-30
int particleFilter(const ParticleFilterCriteria2&, int, const ParticleFilterOptions&, int& );
int particleFilter(const particleFilterCriteria2&, int, const particleFilterOptions&, Rect, int&);
int imaqParticleFilter3(const ParticleFilterCriteria2, int, const ParticleFilterOptions&, const ROI&, int&);
int imaqParticleFilter3(Image* dest, Image* source, const ParticleFilterCriteria2* criteria, int
criteriaCount, const ParticleFilterOptions* options, const ROI* roi, int* numParticles)
/*

Page 44 to Page 47

*/

    int StartCameraTask();
    int StartCameraTask(int, int, ImageSize, ImageRotation);
    int StartCameraTask(int, int, ImageSize, ImageRotation, int);
    int StopCameraTask();
    int StartImageAcquisition(void);




private:
    Image * img;
};

#endif
