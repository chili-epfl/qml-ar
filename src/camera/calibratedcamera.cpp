/**
 * @file calibratedcamera.cpp
 * @brief This class stores information
 * about a calibrated camera
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "calibratedcamera.h"

CalibratedCamera::CalibratedCamera()
{
    A = QMatrix3x3();
    calibration_w = 0;
    calibration_h = 0;
    image_w = 0;
    image_h = 0;
}

CalibratedCamera::CalibratedCamera(CalibratedCamera *that)
{
    Q_ASSERT(that != NULL);
    this->A = that->A;
    this->calibration_w = that->calibration_w;
    this->calibration_h = that->calibration_h;
    this->image_h = that->image_h;
    this->image_w = that->image_w;
}

QMatrix3x3 CalibratedCamera::getMatrixOld()
{
    return A;
}

double CalibratedCamera::getScaler()
{
    Q_ASSERT(calibration_w != 0);
    return (1. * image_w / calibration_w);
}

QMatrix3x3 CalibratedCamera::getMatrix()
{
    // multiplying matrix to the scaler, see
    // https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
    QMatrix3x3 result = A;
    result *= getScaler();
    return(result);
}

void CalibratedCamera::setResolution(int new_width, int new_height)
{
    Q_ASSERT(new_width != 0);
    Q_ASSERT(new_height != 0);
    image_w = new_width;
    image_h = new_height;
#ifdef DEBUG_SHADER
    qDebug() << "camera h=" << image_h << " w=" << image_w;
#endif
}

int CalibratedCamera::width()
{
    return image_w;
}

int CalibratedCamera::height()
{
    return image_h;
}

void CalibratedCamera::setResolutionImage(PipelineContainer<QImage> image)
{
    // reject further calls
    disconnect(this, SLOT(setResolutionImage(PipelineContainer<QImage>)));

    QImage img = image;

    setResolution(img.width(), img.height());
}
