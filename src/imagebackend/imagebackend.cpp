#include "imagebackend.h"
#include <QDebug>

ImageBackend::ImageBackend(QString filename) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    QImage img(filename);
    Q_ASSERT(!img.isNull());
    buffer = img;
}

QPixmap ImageBackend::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return QPixmap::fromImage(buffer);
}
