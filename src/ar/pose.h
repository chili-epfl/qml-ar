#ifndef POSE_H
#define POSE_H

#include <QVector3D>
#include <QVector4D>

/*
 * This class holds the pose (Translation+Rotation)
 * of the camera
 */

class Pose
{
private:
    QVector3D translation;
    QVector4D rotation;
    bool is_valid;
public:
    // construct invalid pose
    Pose();

    // construct valid pose with translation and rotation
    Pose(QVector3D t, QVector4D r);

    // is pose valid
    bool isValid();

    // returns translation vector
    QVector3D getTranslation();

    // return rotation vector
    QVector4D getRotation();
};

#endif // POSE_H
