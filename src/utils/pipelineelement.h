#ifndef PIPELINEELEMENT_H
#define PIPELINEELEMENT_H

#include "worldimage.h"
#include <QImage>
#include <QDateTime>
#include <QVideoFrame>
#include <QMatrix4x4>

/*
 * This class represents an item in the QML-AR threading pipeline
 */

class PipelineElement
{
public:
    QDateTime timestamp;
    QVideoFrame frame;
    QImage image;
    WorldImageCorrespondences correspondences;
    QMatrix4x4 mvp;
};


#endif // PIPELINEELEMENT_H
