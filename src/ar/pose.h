#ifndef POSE_H
#define POSE_H

#include <QVector3D>
#include <QMatrix3x3>

/*
 * This class holds the pose (Translation+Rotation)
 * of the camera
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/Cellulo):
 * x right
 * y down
 * z from reader
 */

class Pose
{
private:
    QVector3D translation;
    QMatrix3x3 rotation;
    bool is_valid;
public:
    // construct invalid pose
    Pose();

    // construct valid pose with translation and rotation
    Pose(QVector3D t, QMatrix3x3 r);

    // is pose valid
    bool isValid();

    // returns translation vector
    QVector3D getTranslation();

    // return rotation vector
    QMatrix3x3 getRotation();

    // return rotation + translation
    QMatrix4x4 get4Matrx();
};

#endif // POSE_H
