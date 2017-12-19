#include "imagebackend.h"

ImageBackend::ImageBackend(QString filename) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    Q_ASSERT(buffer.load(filename));
}

QPixmap ImageBackend::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return QPixmap::fromImage(buffer);
}
