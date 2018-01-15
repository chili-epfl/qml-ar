#ifndef POSEPREDICTOR_H
#define POSEPREDICTOR_H

/*
 * This abstract class represent a predictor for camera pose
 * (rot., transl.)
 */

#include <QObject>
#include "pose.h"

class PosePredictor : public QObject
{ Q_OBJECT
public:
    PosePredictor();
    virtual void setCurrentPose(Pose pose);
    virtual Pose predictPose();
};

#endif // POSEPREDICTOR_H
