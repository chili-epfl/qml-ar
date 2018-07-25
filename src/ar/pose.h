/**
 * @file pose.h
 * @brief This class holds the pose (Translation+Rotation)
 * of the camera
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef POSE_H
#define POSE_H

#include <QVector3D>
#include <QMatrix3x3>
#include <QMatrix4x4>

/**
 * @brief This class holds the pose (Translation+Rotation)
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
    /**
    * @brief Construct invalid pose
    */
    Pose();

    /**
    * @brief Construct using 4x4 matrix
    * @param map Matrix to convert to pose
    */
    Pose(QMatrix4x4 map);

    /**
    * @brief Construct valid pose with translation and rotation
    * @param t Translation
    * @param r Rotation matrix
    */
    Pose(QVector3D t, QMatrix3x3 r);

    /**
    * @brief Is pose valid
    */
    bool isValid();

    /**
    * @brief Returns translation vector
    */
    QVector3D getTranslation();

    /**
    * @brief Return rotation vector
    */
    QMatrix3x3 getRotation();

    /**
    * @brief Return rotation + translation
    */
    QMatrix4x4 get4Matrix();

    /**
    * @brief Get string representation
    */
    QString toString();
};

#endif // POSE_H
