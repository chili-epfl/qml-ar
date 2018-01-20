#include "imagebackend.h"
#include "pipelinethread.h"
#include <QDebug>

ImageBackend::ImageBackend(QString filename)
{
    QImage img(filename);
    Q_ASSERT(!img.isNull());
    last_element.image = img;
    pipelineThread = new PipelineThread(this);
    pipelineThread->setInput(&last_element);
}
