#include "calibratedcamera.h"

CalibratedCamera::CalibratedCamera()
{
    A = QMatrix3x3();
}

QMatrix3x3 CalibratedCamera::getMatrix()
{
    return A;
}
