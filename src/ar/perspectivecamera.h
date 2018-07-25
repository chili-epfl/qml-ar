/**
 * @file perspectivecamera.h
 * @brief This class extends CalibratedCamera
 * by allowing to get Perspective Projection matrix
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "calibratedcamera.h"
#include <QObject>
#include <QMatrix4x4>

/**
 * @brief This class extends CalibratedCamera
 * by allowing to get Perspective Projection matrix
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
 */

class PerspectiveCamera : public CalibratedCamera
{ Q_OBJECT
public:
    /**
    * @brief Initialize with a calibrated camera
    * @param camera CalibratedCamera object pointer
    */
    PerspectiveCamera(CalibratedCamera* camera);

    virtual ~PerspectiveCamera() {}

    /**
    * @brief Returns perspective projection matrix obtained from camera matrix
    * @param n Near, world units (mm)
    * @param f Far, world units (mm)
    */
    QMatrix4x4 getPerspectiveMatrix(double n, double f);
};

#endif // PERSPECTIVECAMERA_H
