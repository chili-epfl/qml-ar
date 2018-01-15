#include "pose.h"
#include <QMatrix4x4>

Pose::Pose()
{
    is_valid = false;
}

Pose::Pose(QMatrix4x4 map)
{
    for(int i = 0; i < 3; i++)
    {
        translation[i] = map(i, 3);
        for(int j = 0; j < 3; j++)
            rotation(i, j) = map(i, j);
    }
    is_valid = true;
}

Pose::Pose(QVector3D t, QMatrix3x3 r)
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

QMatrix3x3 Pose::getRotation()
{
    return rotation;
}

QMatrix4x4 Pose::get4Matrix()
{
    QMatrix4x4 res;
    res.fill(0);
    for(int i = 0; i < 3; i++)
    {
        res(i, 3) = translation[i];
        for(int j = 0; j < 3; j++)
            res(i, j) = rotation(i, j);
    }

    res(3, 3) = 1;

    return res;
}
