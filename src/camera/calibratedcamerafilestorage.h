/**
 * @file calibratedcamerafilestorage.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CALIBRATEDCAMERAFILESTORAGE_H
#define CALIBRATEDCAMERAFILESTORAGE_H

#include "calibratedcamera.h"
#include "configjson.h"

/*
 * This class implements CalibratedCamera
 * and allows to load information from a file
 *
 *
 * The file should contain Camera Matrix, or the matrix of intrinsic parameters:
 * https://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html
 *
 * The world coordinate system should be in millimeters
 *
 * Example file (should contain 9 floating point numbers, order: row-major):
 * {
 *     "camera_matrix": [
 *         7.3321993364036177e+02, 0., 2.9429448615773646e+02, 0.,
 *         7.3321993364036177e+02, 6.8074088355361582e+01, 0., 0., 1.
 *     ],
 *     "width": 640,
 *     "height": 480
 * }
 *
 */


class CalibratedCameraFileStorage : public CalibratedCamera, public ConfigJSON
{ Q_OBJECT
public:
    /**
    * @brief Empty constructor (need to populate later)
    */
    CalibratedCameraFileStorage() : CalibratedCamera() {}

    virtual ~CalibratedCameraFileStorage() {}

    /**
    * @brief Read config on creation
    */
    CalibratedCameraFileStorage(QString filename);

    // fill with data from a file/other QIODevice
    void readConfig(QJsonObject config);
};

#endif // CALIBRATEDCAMERAFILESTORAGE_H
