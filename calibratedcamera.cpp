#include "calibratedcamera.h"
#include <QCameraLens>

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
    return getPerspectiveMatrixMethod1(n, f, l, r, b, t);
}

QMatrix4x4 CalibratedCamera::getPerspectiveMatrixMethod1(float n, float f, float l, float r, float b, float t)
{
    // get matrix from the camera projection matrix (calibrated)
    // see http://ksimek.github.io/2013/06/03/calibrated_cameras_in_opengl/
    QMatrix4x4 persp;
    persp(0, 0) = alpha;
    persp(0, 1) = s;
    persp(0, 2) = c_x;
    persp(0, 3) = 0;

    persp(1, 0) = 0;
    persp(1, 1) = beta;
    persp(1, 2) = c_y;
    persp(1, 3) = 0;

    persp(2, 0) = 0;
    persp(2, 1) = 0;
    persp(2, 2) = n + f;
    persp(2, 3) = n * f;

    persp(3, 0) = 0;
    persp(3, 1) = 0;
    persp(3, 2) = -1;
    persp(3, 3) = 0;

    Qt3DRender::QCameraLens lens;
    lens.setOrthographicProjection(l, r, b, t, n, f);

    return lens.projectionMatrix() * persp;
}

QMatrix4x4 CalibratedCamera::getPerspectiveMatrixMethod2(float n, float f, float l, float r, float b, float t)
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
