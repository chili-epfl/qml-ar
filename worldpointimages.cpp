#include "worldpointimages.h"
#include <QMatrix4x4>

WorldPointImages::WorldPointImages()
{

}

void WorldPointImages::assertValid()
{
    Q_ASSERT(world_points.size() == image_points.size());
}

void WorldPointImages::add(QVector3D world_point, QVector2D image_point)
{
    world_points.append(world_point);
    image_points.append(image_point);
    assertValid();
}

void WorldPointImages::join(WorldPointImages &that)
{
    int length = that.world_points.size();
    for(int i = 0; i < length; i++)
    {
        world_points.append(that.world_points.at(i));
        image_points.append(that.image_points.at(i));
    }
    assertValid();
}

void WorldPointImages::clear()
{
    world_points.clear();
    image_points.clear();
    assertValid();
}

int WorldPointImages::size()
{
    return world_points.size();
}

QMatrix4x4 WorldPointImages::computePnP()
{
    Q_ASSERT(size() > 0);
    return QMatrix4x4();
}
