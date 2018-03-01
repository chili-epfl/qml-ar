#ifndef CALIBRATEDCAMERA_H
#define CALIBRATEDCAMERA_H

#include <QMatrix3x3>
#include <QObject>
#include <QImage>
#include "pipelinecontainer.h"

/*
 * This class stores information
 * about a calibrated camera
 */

class CalibratedCamera : public QObject
{ Q_OBJECT
protected:
    // camera matrix
    // see https://docs.opencv.org/3.0-beta/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
    QMatrix3x3 A;

    // width and height of the images in pixels
    // when the camera was calibrated
    int calibration_w, calibration_h;

    // current image width and height in pixels
    int image_w, image_h;

public:
    // initialize matrix with eye matrix
    CalibratedCamera();

    virtual ~CalibratedCamera() {}

    // initialize with another CalibratedCamera
    // (CalibratedCameraFileStorage, for example)
    CalibratedCamera(CalibratedCamera* that);

    // get camera matrix
    // assuming that the resolution changed
    QMatrix3x3 getMatrix();

    // set the current image resolution in pixels
    void setResolution(int new_width, int new_height);

    // get image size
    int width();
    int height();

public slots:
    void setResolutionImage(PipelineContainer<QImage> image);

private:
    // get camera matrix A
    // assuming the resolution did not change
    QMatrix3x3 getMatrixOld();

    // returns the scaler from calibration resolution (calibration_w, calibration_h)
    // to the new resolution (image_w, image_h)
    // the new matrix is A * getScaler()
    double getScaler();
};

#endif // CALIBRATEDCAMERA_H
