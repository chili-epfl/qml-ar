#ifndef POSE_H
#define POSE_H

#include <QVector3D>
#include <QMatrix3x3>
#include <QMatrix4x4>

/*
 * This class holds the pose (Translation+Rotation)
 * of the camera
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
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

    // construct using 4x4 matrix
    Pose(QMatrix4x4 map);

    // construct valid pose with translation and rotation
    Pose(QVector3D t, QMatrix3x3 r);

    // is pose valid
    bool isValid();

    // returns translation vector
    QVector3D getTranslation();

    // return rotation vector
    QMatrix3x3 getRotation();

    // return rotation + translation
    QMatrix4x4 get4Matrix();

    // get string representation
    QString toString();
};

#endif // POSE_H
