#ifndef MARKERMVPPROVIDER_H
#define MARKERMVPPROVIDER_H

#include "mvpprovider.h"
#include "markerdetector.h"
#include "calibratedcamera.h"

/*
 *
 * This class implements MVPProvider
 * using pose from a detected markers,
 * a calibrated camera's matrix
 * multiplies them and outputs MVP
 *
 */

class MarkerMVPProvider : public MVPProvider
{
private:
    CalibratedCamera* camera;
    MarkerDetector* detector;
public:
    MarkerMVPProvider(MarkerDetector *d, CalibratedCamera *c);

    QMatrix4x4 getP();
    QMatrix4x4 getMV();
public slots:
    void recompute();
};

#endif // MARKERMVPPROVIDER_H
