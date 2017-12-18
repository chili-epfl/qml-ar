#include "worldpointimages.h"

WorldPointImages::WorldPointImages()
{

}

void WorldPointImages::add(QVector3D world_point, QVector2D image_point)
{
    world_points.append(world_point);
    image_points.append(image_point);
}

void WorldPointImages::clear()
{
    world_points.clear();
    image_points.clear();
}
