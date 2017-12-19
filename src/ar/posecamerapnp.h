#ifndef CAMERAPOSEESTIMATORCORRESPONDENCES_H
#define CAMERAPOSEESTIMATORCORRESPONDENCES_H

#include "pose.h"
#include "worldimage.h"
#include "calibratedcamera.h"

/*
 * This class converts camera + 3D-2D correspondences
 * to a camera pose
 */

class CameraPoseEstimatorCorrespondences
{
public:
    // returns inverse pose of the camera
    static Pose estimate(CalibratedCamera* camera, WorldImageCorrespondences* correspondences);
};

#endif // CAMERAPOSEESTIMATORCORRESPONDENCES_H
