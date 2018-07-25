/**
 * @file posecamerapnp.h
 * @brief This class converts camera + 3D-2D correspondences
 * to a camera pose
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CAMERAPOSEESTIMATORCORRESPONDENCES_H
#define CAMERAPOSEESTIMATORCORRESPONDENCES_H

#include "pose.h"
#include "worldimage.h"
#include "calibratedcamera.h"

/**
 * @brief This class converts camera + 3D-2D correspondences
 * to a camera pose
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

class CameraPoseEstimatorCorrespondences
{
public:
    /**
    * @brief Returns inverse pose of the camera
    * @param camera Calibrated camera parameters
    * @param correspondences Correspondences between world points and image points
    */
    static Pose estimate(CalibratedCamera* camera, WorldImageCorrespondences* correspondences);
};

#endif // CAMERAPOSEESTIMATORCORRESPONDENCES_H
