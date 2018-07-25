/**
 * @file posefilter.h
 * @brief This class implements low-pass filter on Pose (Rotation + Translation)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef POSEFILTER_H
#define POSEFILTER_H

#include "pose.h"
#include "mvpprovider.h"

/**
 * @brief This class implements low-pass filter on Pose (Rotation + Translation)
 *
 * Rotation filtering is done by (q1 + q1).normalize() for quaternions
 * This solution is approximate, see
 * https://stackoverflow.com/questions/21241965/average-transformation-matrix-for-a-list-of-transformations
 */

class PoseFilter : public MVPProvider
{ Q_OBJECT
private:
    /**
    * @brief Cached p and mv matrices
    * (input)
    */
    QMatrix4x4 last_mv;

    /**
    * @brief Filtering parameter
    */
    double alpha;

    /**
    * @brief Is now first update?
    */
    bool first_update;

    /**
    * @brief Last rotation from filter
    */
    QQuaternion last_filter_r;

    /**
    * @brief Last translation from filter
    */
    QVector3D last_filter_t;
public:
    /**
    * @brief Initialize with alpha
    * Out = real * alpha + last_filtered * (1 - alpha)
    */
    PoseFilter(double alpha = 0.5);

public slots:
    /**
    * @brief Set p matrix which to decorate
    */
    void setP(PipelineContainer<QMatrix4x4> p);

    /**
    * @brief Set mv matrix which to decorate
    */
    void setMV(PipelineContainer<QMatrix4x4> mv);

    /**
    * @brief Recalculate filter output
    */
    void update();

    /**
    * @brief New filtering alpha
    * @param alpha Low-pass filter coefficient: val = old * (1 - alpha) + new * alpha
    */
    void setAlpha(double alpha);
};

#endif // POSEFILTER_H
