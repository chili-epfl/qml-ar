#include "perspectivecamera.h"
#include <QCameraLens>

PerspectiveCamera::PerspectiveCamera(CalibratedCamera *camera) : CalibratedCamera(camera)
{
}

QMatrix4x4 PerspectiveCamera::getPerspectiveMatrix(double n, double f)
{
    // res =
    // A00 A01 A02 0
    // A10 A11 A12 0
    // 0    0   1  1
    // 0    0   1  0

    // z depth information is lost

    QMatrix4x4 res;
    res.fill(0);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            res(i, j) = A(i, j);

    res(2, 2) = 0;
    res(3, 2) = 1;

    // mapping z values to clipping space
    res(2, 2) = (f + n) / (f - n);
    res(2, 3) = -2 * n * f / (f - n);
    return res;
}
