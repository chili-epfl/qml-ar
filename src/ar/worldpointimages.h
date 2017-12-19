#ifndef WORLDIMAGECORRESPONDENCES_H
#define WORLDIMAGECORRESPONDENCES_H

#include <QVector3D>
#include <QVector2D>
#include "calibratedcamera.h"
#include <QVector>

/*
 * This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
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

    // return 4x4 ModelView matrix from point correspondences
    // and a calibrated camera matrix
    QMatrix4x4 computePnP(CalibratedCamera* camera);
};

#endif // WORLDIMAGECORRESPONDENCES_H
