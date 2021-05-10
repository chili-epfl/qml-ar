/**
 * @file perspectivecamera.cpp
 * @brief This class extends CalibratedCamera
 * by allowing to get Perspective Projection matrix
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "perspectivecamera.h"
#include <QCameraLens>
#include <QDebug>

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

    QMatrix3x3 camera_matrix = getMatrix();

    QMatrix4x4 res;
    res.fill(0);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            res(i, j) = camera_matrix(i, j);

    res(2, 2) = 0;
    res(3, 2) = 1;

    // mapping z values to clipping space
    // required for proper texture mapping
    // using z-depth buffer
    // equation: resulting_z = (res(2, 2) * source_z + res(2, 3)) / source_z

    res(2, 2) = (f + n) / (f - n);
    res(2, 3) = -2 * n * f / (f - n);
#ifdef DEBUG_SHADER
    qDebug() << "fn" << f << n << camera_matrix;
#endif
    return res;
}
