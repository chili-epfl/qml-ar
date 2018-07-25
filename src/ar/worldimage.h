/**
 * @file worldimage.h
 * @brief This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef WORLDIMAGECORRESPONDENCES_H
#define WORLDIMAGECORRESPONDENCES_H

#include <QVector3D>
#include <QVector2D>
#include "calibratedcamera.h"
#include "pose.h"
#include <QVector>

/**
 * @brief This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

class WorldImageCorrespondences
{
private:
    /**
    * @brief Points in 3D
    */
    QVector<QVector3D> world_points;

    /**
    * @brief Corresponding points in 2D
    */
    QVector<QVector2D> image_points;

    /**
    * @brief Check if internal structure is valid
    */
    void assertValid();
public:
    WorldImageCorrespondences();

    /**
    * @brief Add a single correspondence
    * @param world_point Vector representing a point in world coordinate system
    * @param image_point Vector representing a point in image coordinate system
    */
    void add(QVector3D world_point, QVector2D image_point);

    /**
    * @brief Merge with another list of correspondences
    * @param that Other object of same type whose data to take
    */
    void join(WorldImageCorrespondences& that);

    /**
    * @brief Remove all correspondences
    */
    void clear();

    /**
    * @brief Number of correspondences
    */
    int size();

    /**
    * @brief Returns world point at index i
    * @param index Index starting from 0
    */
    QVector3D getWorldPoint(int index);

    /**
    * @brief Returns image point at index i
    * @param index Index starting from 0
    */
    QVector3D getImagePoint(int index);
};

#endif // WORLDIMAGECORRESPONDENCES_H
