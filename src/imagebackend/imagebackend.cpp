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
    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &ImageBackend::update);
    timer.start();
}

ImageBackend::~ImageBackend()
{

}

void ImageBackend::update()
{
    // no action if not running
    if(!is_active) return;

    // scale image?
    static bool scale = false;
    QImage buf1 = buffer;

    // scale image to make it a bit different
    if(scale) buf1 = buffer.scaled(buf1.width() * 0.9, buf1.height() * 0.9);

    PipelineContainerInfo info(image_id++);
    emit imageAvailable(PipelineContainer<QImage>
                        (buf1, info.checkpointed("Sent")));

    // toggling scale
    scale = !scale;
}

QImage ImageBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buffer;
}
