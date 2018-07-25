/**
 * @file calibratedcamera.h
 * @brief This class stores information
 * about a calibrated camera
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CALIBRATEDCAMERA_H
#define CALIBRATEDCAMERA_H

#include <QMatrix3x3>
#include <QObject>
#include <QImage>
#include "pipelinecontainer.h"

/**
 * @brief This class stores information
 * about a calibrated camera
 */

class CalibratedCamera : public QObject
{ Q_OBJECT
protected:
    /**
    * @brief Camera matrix
    */
    // see https://docs.opencv.org/3.0-beta/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
    QMatrix3x3 A;

    /**
    * @brief Width and height of the images in pixels
    * When the camera was calibrated
    */
    int calibration_w, calibration_h;

    /**
    * @brief Current image width and height in pixels
    */
    int image_w, image_h;

public:
    /**
    * @brief Initialize matrix with eye matrix
    */
    CalibratedCamera();

    virtual ~CalibratedCamera() {}

    /**
    * @brief Initialize with another CalibratedCamera
    * (CalibratedCameraFileStorage, for example)
    * @param that Other instance of CalibratedCamera
    */
    CalibratedCamera(CalibratedCamera* that);

    /**
    * @brief Get camera matrix
    * Assuming that the resolution changed
    */
    QMatrix3x3 getMatrix();

    /**
    * @brief Set the current image resolution in pixels
    * @param new_width Width in pixels
    * @param new_height Height in pixels
    */
    void setResolution(int new_width, int new_height);

    /**
    * @brief Get image width
    */
    int width();

    /**
    * @brief Get image height
    */
    int height();

public slots:

    /**
    * @brief Set resolution from QImage
    * @param image QImage to process
    */
    void setResolutionImage(PipelineContainer<QImage> image);

private:
    /**
    * @brief Get camera matrix A
    * Assuming the resolution did not change
    */
    QMatrix3x3 getMatrixOld();

    /**
    * @brief Returns the scaler from calibration resolution (calibration_w, calibration_h)
    * To the new resolution (image_w, image_h)
    * The new matrix is A * getScaler()
    */
    double getScaler();
};

#endif // CALIBRATEDCAMERA_H
