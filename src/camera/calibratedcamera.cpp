#include "calibratedcamera.h"

CalibratedCamera::CalibratedCamera()
{
    A = QMatrix3x3();
}

CalibratedCamera::CalibratedCamera(CalibratedCamera *that)
{
    Q_ASSERT(that != NULL);
    this->A = that->A;
}

QMatrix3x3 CalibratedCamera::getMatrix()
{
    return A;
}
