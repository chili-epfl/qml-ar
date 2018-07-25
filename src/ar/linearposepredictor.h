/**
 * @file linearposepredictor.h
 * @brief This class implements linear Pose Predictor
 * which assumes angular/linear velocities to be constant
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef LINEARPOSEPREDICTOR_H
#define LINEARPOSEPREDICTOR_H

#include <QDateTime>
#include "posepredictor.h"

/**
 * This class implements linear Pose Predictor
 * which assumes angular/linear velocities to be constant
 */

class LinearPosePredictor : public PosePredictor
{
private:
    /**
    * @brief Two time moments
    */
    QDateTime t1, t2;

    /**
    * @brief Two poses at those time moments
    */
    Pose p1, p2;

    /**
    * @brief Must be equal to two to be valid
    */
    int is_valid;
public:
    LinearPosePredictor();

    /**
    * @brief Set pose at this moment
    * @param pose pose at this moment
    */
    void setCurrentPose(Pose pose);

    /**
    * @brief Estimate pose at this moment
    */
    Pose predictPose();
};

#endif // LINEARPOSEPREDICTOR_H


