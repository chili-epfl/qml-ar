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
    Q_ASSERT(!img.isNull());
    buffer = img;
    emit imageAvailable(buffer);
}

QImage ImageBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buffer;
}
