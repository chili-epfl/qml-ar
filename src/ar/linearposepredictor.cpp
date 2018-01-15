#include <QQuaternion>
#include "linearposepredictor.h"

LinearPosePredictor::LinearPosePredictor()
{
    // initially, have 0 points
    is_valid = 0;
}

void LinearPosePredictor::setCurrentPose(Pose pose)
{
    // second pose is now first pose
    p1 = p2;
    t1 = t2;

    // new pose is second pose
    p2 = pose;
    t2 = QDateTime::currentDateTime();

    // updating is_valid at the beginning
    if(is_valid < 2)
        is_valid++;
}

Pose LinearPosePredictor::predictPose()
{
    if(is_valid == 0)
        // no pose if nothing was set
        return Pose();
    else if(is_valid == 1)
        // first item if only it is available
        return p1;

    // linear scaler if two poses are available
    double scaler = t1.msecsTo(t2);
    scaler /= t1.msecsTo(QDateTime::currentDateTime());

    // new translation
    QVector3D new_translation = p1.getTranslation() + (p2.getTranslation() - p1.getTranslation()) * scaler;

    // two rotation quaternions
    QQuaternion p1_rot = QQuaternion::fromRotationMatrix(p1.getRotation());
    QQuaternion p2_rot = QQuaternion::fromRotationMatrix(p2.getRotation());

    // delta quaternion
    QQuaternion rot_delta = p2_rot * p1_rot.inverted();

    // obtaining axis and angle
    QVector3D rot_axis;
    float rot_angle;
    rot_delta.getAxisAndAngle(&rot_axis, &rot_angle);

    // multiplying angle by scaler
    QQuaternion new_rotation = QQuaternion::fromAxisAndAngle(rot_axis, rot_angle * scaler) * p1_rot;

    // returning resulting scaled pose
    return Pose(new_translation, new_rotation.toRotationMatrix());
}
