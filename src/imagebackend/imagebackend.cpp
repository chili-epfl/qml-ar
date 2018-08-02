/**
 * @file imagebackend.cpp
 * @brief This class implements a QQuickImageProvider
 * showing a constant image from a file
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "imagebackend.h"
#include <QDebug>

ImageBackend::ImageBackend(QString filename) : ImageProviderAsync()
{
    QImage img(filename);
    image_id = 0;
    Q_ASSERT(!img.isNull());
    buffer = img.convertToFormat(QImage::Format_RGB888);
    timer.setInterval(100);
    connect(&timer, &QTimer::timeout, this, &ImageBackend::update);
    timer.start();
}

ImageBackend::~ImageBackend()
{

}

void ImageBackend::update()
{
    qDebug() << "SENT";
    PipelineContainerInfo info(image_id++);
    emit imageAvailable(PipelineContainer<QImage>
                        (buffer, info.checkpointed("Sent")));
}

QImage ImageBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buffer;
}
