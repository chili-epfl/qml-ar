#include "calibratedcamera.h"

CalibratedCamera::CalibratedCamera()
{
    A = QMatrix3x3();
}

QMatrix3x3 CalibratedCamera::getMatrix()
{
    return A;
}

QMatrix4x4 CalibratedCamera::getPerspectiveMatrix(float n, float f, float l, float r, float b, float t)
{
    // get matrix from the camera projection matrix (calibrated)
    // see http://kgeorge.github.io/2014/03/08/calculating-opengl-perspective-matrix-from-opencv-intrinsic-matrix
    float alpha = A(0, 0);
    float beta  = A(1, 1);
    float c_x   = A(0, 2);
    float c_y   = A(1, 2);

    QMatrix4x4 res;
    res(0, 0) = alpha / c_x;
    res(1, 1) = beta / c_y;
    res(2, 2) = -(f + n)/(f - n);
    res(3, 3) = 0;
    res(2, 3) = -2 * f * n / (f - n);
    res(3, 2) = -1;
    res(0, 2) = (r + l) / (r - l);
    res(1, 2) = (t + b) / (t - b);

    return res.transposed();
}
