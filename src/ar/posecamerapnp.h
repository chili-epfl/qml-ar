#ifndef CAMERAPOSEESTIMATORCORRESPONDENCES_H
#define CAMERAPOSEESTIMATORCORRESPONDENCES_H

#include "pose.h"
#include "worldimage.h"
#include "calibratedcamera.h"

class CameraPoseEstimatorCorrespondences
{
public:
    static Pose estimate(CalibratedCamera* camera, WorldImageCorrespondences* correspondences);
};

#endif // CAMERAPOSEESTIMATORCORRESPONDENCES_H
