#ifndef MARKERMVPPROVIDER_H
#define MARKERMVPPROVIDER_H

#include "mvpprovider.h"
#include "markerdetector.h"
#include "perspectivecamera.h"

/*
 * This class implements MVPProvider
 * using pose from a detected markers,
 * a calibrated camera's matrix
 * multiplies them and outputs MVP
 *
 * INPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 *
 * OUTPUT COORDINATE SYSTEM (OpenGL)
 * x right
 * y up
 * z to reader
 *
 */

class MarkerMVPProvider : public MVPProvider
{ Q_OBJECT
private:
    PerspectiveCamera* camera;

    // near and far are in millimeters (positive)
    QMatrix4x4 getP(double n = 1, double f = 10000);
    QMatrix4x4 getMV(MarkerStorage storage);

public:
    MarkerMVPProvider(PerspectiveCamera *c);

public slots:
    // this functions actually compute matrices
    void recompute(MarkerStorage storage);
};

#endif // MARKERMVPPROVIDER_H
