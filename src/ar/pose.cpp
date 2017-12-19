#include "pose.h"

Pose::Pose()
{
    is_valid = false;
}

Pose::Pose(QVector3D t, QVector4D r)
{
    translation = t;
    rotation = r;
    is_valid = true;
}

bool Pose::isValid() {
    return is_valid;
}

QVector3D Pose::getTranslation()
{
    return translation;
}

QVector4D Pose::getRotation()
{
    return rotation;
}
