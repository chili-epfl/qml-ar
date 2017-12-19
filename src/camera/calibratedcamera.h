#ifndef CALIBRATEDCAMERA_H
#define CALIBRATEDCAMERA_H

#include <QMatrix3x3>

/*
 * This class stores information
 * about a calibrated camera
 */

class CalibratedCamera
{
protected:
    // camera matrix
    // see https://docs.opencv.org/3.0-beta/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
    QMatrix3x3 A;

public:
    // initialize matrix with eye matrix
    CalibratedCamera();

    // initialize with another CalibratedCamera
    // (CalibratedCameraFileStorage, for example)
    CalibratedCamera(CalibratedCamera* that);

    // get camera matrix
    QMatrix3x3 getMatrix();
};

#endif // CALIBRATEDCAMERA_H
