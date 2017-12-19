#ifndef WORLDIMAGECORRESPONDENCES_H
#define WORLDIMAGECORRESPONDENCES_H

#include <QVector3D>
#include <QVector2D>
#include "calibratedcamera.h"
#include "pose.h"
#include <QVector>

/*
 * This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/Cellulo):
 * x right
 * y down
 * z from reader
 */

class WorldImageCorrespondences
{
private:
    // points in 3D
    QVector<QVector3D> world_points;

    // corresponding points in 2D
    QVector<QVector2D> image_points;

    // check if internal structure is valid
    void assertValid();
public:
    WorldImageCorrespondences();

    // add a single correspondence
    void add(QVector3D world_point, QVector2D image_point);

    // merge with another list of correspondences
    void join(WorldImageCorrespondences& that);

    // remove all correspondences
    void clear();

    // number of correspondences
    int size();

    // returns world point at index i
    QVector3D getWorldPoint(int index);

    // returns image point at index i
    QVector3D getImagePoint(int index);
};

#endif // WORLDIMAGECORRESPONDENCES_H
