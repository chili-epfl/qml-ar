#ifndef POSEFILTER_H
#define POSEFILTER_H

#include "pose.h"
#include "mvpprovider.h"

/*
 * This class implements low-pass filter on Pose (Rotation + Translation)
 * Rotation filtering is done by (q1 + q1).normalize() for quaternions
 * This solution is approximate, see
 * https://stackoverflow.com/questions/21241965/average-transformation-matrix-for-a-list-of-transformations
 */

class PoseFilter : public MVPProvider
{ Q_OBJECT
private:
    // cached p and mv matrices
    // (input)
    QMatrix4x4 last_mv;

    // filtering parameter
    double alpha;

    // is now first update?
    bool first_update;

    // last rotation from filter
    QQuaternion last_filter_r;

    // last translation from filter
    QVector3D last_filter_t;
public:
    // initialize with alpha
    // out = real * alpha + last_filtered * (1 - alpha)
    PoseFilter(double alpha = 0.5);

public slots:
    // set matrices which to decorate
    void setP(QMatrix4x4 p);
    void setMV(QMatrix4x4 mv);

    // recalculate filter output
    void update();

    // new filtering alpha
    void setAlpha(double alpha);
};

#endif // POSEFILTER_H
