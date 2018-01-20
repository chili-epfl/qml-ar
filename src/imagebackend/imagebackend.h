#ifndef IMAGEBACKEND_H
#define IMAGEBACKEND_H

#include <QFile>
#include "abstractcamerabackend.h"

/*
 * This class implements a QQuickImageProvider
 * showing a constant image from a file
 */

class ImageBackend : public AbstractCameraBackend
{
private:
    PipelineElement last_element;
public:
    // load image from the file
    ImageBackend(QString filename);
};

#endif // IMAGEBACKEND_H
