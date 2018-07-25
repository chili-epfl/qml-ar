/**
 * @file worldimage.cpp
 * @brief This class stores the correspondences
 * between Real World 3D points
 * and projected points on camera image
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "worldimage.h"
#include "calibratedcamera.h"

WorldImageCorrespondences::WorldImageCorrespondences()
{

}

void WorldImageCorrespondences::assertValid()
{
    Q_ASSERT(world_points.size() == image_points.size());
}

void WorldImageCorrespondences::add(QVector3D world_point, QVector2D image_point)
{
    world_points.append(world_point);
    image_points.append(image_point);
    assertValid();
}

void WorldImageCorrespondences::join(WorldImageCorrespondences &that)
{
    int length = that.world_points.size();
    for(int i = 0; i < length; i++)
    {
        world_points.append(that.world_points.at(i));
        image_points.append(that.image_points.at(i));
    }
    assertValid();
}

void WorldImageCorrespondences::clear()
{
    world_points.clear();
    image_points.clear();
    assertValid();
}

int WorldImageCorrespondences::size()
{
    return world_points.size();
}

QVector3D WorldImageCorrespondences::getWorldPoint(int index)
{
    Q_ASSERT(index >= 0 && index < size());
    return QVector3D(world_points.at(index));
}

QVector3D WorldImageCorrespondences::getImagePoint(int index)
{
    Q_ASSERT(index >= 0 && index < size());
    return QVector3D(image_points.at(index));
}
