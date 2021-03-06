/**
 * @file posefilter.cpp
 * @brief This class implements low-pass filter on Pose (Rotation + Translation)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "posefilter.h"

PoseFilter::PoseFilter(double alpha)
{
    // save alpha
    this->alpha = alpha;

    // no updates took place
    first_update = true;
}

void PoseFilter::setP(PipelineContainer<QMatrix4x4> p)
{
    p_matrix = p;
}

void PoseFilter::setMV(PipelineContainer<QMatrix4x4> mv)
{
    last_mv = mv;
    object_in_process = mv.info();
    update();
}

void PoseFilter::update()
{
    // pose from last MV matrix
    Pose p(last_mv);

    // alpha is 1 on first update
    double use_alpha = first_update ? 1.0 : alpha;
    if(first_update) first_update = false;

    // input rot quaternion
    QQuaternion rotation = QQuaternion::fromRotationMatrix(p.getRotation());

    // updating t and r
    last_filter_t =  last_filter_t * (1 - use_alpha) + use_alpha * p.getTranslation();
    last_filter_r = (last_filter_r * (1 - use_alpha) + use_alpha * rotation          ).normalized();

    // calculating new pose matrix
    Pose new_pose(last_filter_t, last_filter_r.toRotationMatrix());

    // saving MV matrix
    mv_matrix = new_pose.get4Matrix();

    // saving MVP matrix
    setMVPMatrix(p_matrix * mv_matrix);
}

void PoseFilter::setAlpha(double alpha)
{
    this->alpha = alpha;
}
