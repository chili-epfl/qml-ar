#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "calibratedcamera.h"
#include <QObject>
#include <QMatrix4x4>

/*
 * This class extends CalibratedCamera
 * by allowing to get Perspective Projection matrix
 */

class PerspectiveCamera : public CalibratedCamera
{
public:
    // initialize with a calibrated camera
    PerspectiveCamera(CalibratedCamera* camera);

    // returns perspective projection matrix obtained from camera matrix
    QMatrix4x4 getPerspectiveMatrix(float n, float f, float l, float r, float b, float t);

    // method I for getPerspectiveMatrix
    QMatrix4x4 getPerspectiveMatrixMethod1(float n, float f, float l, float r, float b, float t);

    // method II for getPerspectiveMatrix
    QMatrix4x4 getPerspectiveMatrixMethod2(float n, float f, float l, float r, float b, float t);
};

#endif // PERSPECTIVECAMERA_H
