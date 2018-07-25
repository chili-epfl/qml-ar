/**
 * @file posepredictor.h
 * @brief This abstract class represent a predictor for camera pose
 * (rot., transl.)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef POSEPREDICTOR_H
#define POSEPREDICTOR_H

/**
 * @brief This abstract class represent a predictor for camera pose
 * (rot., transl.)
 */

#include <QObject>
#include "pose.h"

class PosePredictor : public QObject
{ Q_OBJECT
public:

    /**
    * @brief Construct pose predictor
    */
    PosePredictor();

    /**
    * @brief Set pose at this moment
    * @param pose Current pose at this moment
    */
    virtual void setCurrentPose(Pose pose) {Q_UNUSED(pose)}

    /**
    * @brief Get pose at this moment via estimation
    */
    virtual Pose predictPose() {return Pose();}

    /**
    * @brief Destroy pose predictor
    */
    virtual ~PosePredictor() {}
};

#endif // POSEPREDICTOR_H
