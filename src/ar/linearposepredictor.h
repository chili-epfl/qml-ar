#ifndef LINEARPOSEPREDICTOR_H
#define LINEARPOSEPREDICTOR_H

#include <QDateTime>
#include "posepredictor.h"

/*
 * This class implements linear Pose Predictor
 * which assumes angular/linear velocities to be constant
 */

class LinearPosePredictor : public PosePredictor
{
private:
    // two time moments
    QDateTime t1, t2;

    // two poses at those time moments
    Pose p1, p2;

    // must be equal to two to be valid
    int is_valid;
public:
    LinearPosePredictor();

    // set pose at this moment
    void setCurrentPose(Pose pose);

    // estimate pose at this moment
    Pose predictPose();
};

#endif // LINEARPOSEPREDICTOR_H
