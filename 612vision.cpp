#include "612vision.h"
#include <nivision.h>
//do pages 24-30
CImage::CImage(){
   img = frcCreateImage(4);
}

CImage::CImage(ImageType type){
   img = frcCreateImage(type);
}

CImage::CImage(const CImage& other){
   img = frcCreateImage(other.type);
   frcCopyImage(img, other.img);
}

CImage::~CImage(){
   frcDispose((void*)img);
}

CImage::int particleFilter(const ParticleFilterCriteria2& criteria, int criteriaCount, const ParticleFilterOptions& options, int& numParticles){
    return  frcParticleFilter(this, this, *criteria, criteriaCount, options, *numParticles);
}

CImage::int particleFilter(const particleFilterCriteria2& criteria, int criteriaCount, const particleFilterOptions& options, Rect rect, int& numParticles){
    return frcParticleFilter(this, this, criteria, criteriaCount, options, rect, numParticles);
}

CImage::int cimaqParticleFilter3(const particleFilterCriteria2 criteria, int criteriaCount, const ParticleFilterOptions& option, const ROI& roi, int& numParticles){
    return imaqParticleFilter3
int imaqParticleFilter3(const ParticleFilterCriteria2, int, const ParticleFilterOptions&, const ROI&, int&);
int imaqParticleFilter3(Image* dest, Image* source, const ParticleFilterCriteria2* criteria, int
criteriaCount, const ParticleFilterOptions* options, const ROI* roi, int* numParticles)


//Weyland:
CImage::crop(Rect rect){
    return frcCrop(this, this,rect);
}


CImage::startCameraTask()

}

CImage::int startCameraTask(int frames, int compression, ImageSize resolution, ImageRotation rotation){
    return StartCameraTask(frames, compression, resolution, rotation);
}

CImage::startCameraTask(int frames, int compression, ImageSize resolution, ImageRotation rotation, int decode){
    return StartCameraTask(frames, compression, resolution, rotation, decode);
}

CImage::stopCameraTask(){
    return frcStopCameraTask;
}
