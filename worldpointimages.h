#ifndef WORLDPOINTIMAGES_H
#define WORLDPOINTIMAGES_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>

/*
 * This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
 */

class WorldPointImages
{
private:
    // points in 3D
    QVector<QVector3D> world_points;

    // corresponding points in 2D
    QVector<QVector2D> image_points;

    // check if internal structure is valid
    void assertValid();
public:
    WorldPointImages();

    // add a single correspondence
    void add(QVector3D world_point, QVector2D image_point);

    // merge with another list of correspondences
    void join(WorldPointImages& that);

    // remove all correspondences
    void clear();

    // number of correspondences
    int size();

    QMatrix4x4 computePnP();
};

#endif // WORLDPOINTIMAGES_H
