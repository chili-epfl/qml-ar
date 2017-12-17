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
    QMatrix3x3 A;
public:
    CalibratedCamera();
    QMatrix3x3 getMatrix();
};

#endif // CALIBRATEDCAMERA_H
