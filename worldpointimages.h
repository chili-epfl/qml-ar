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
    QVector<QVector3D> world_points;
    QVector<QVector2D> image_points;
public:
    WorldPointImages();

    void add(QVector3D world_point, QVector2D image_point);
};

#endif // WORLDPOINTIMAGES_H
