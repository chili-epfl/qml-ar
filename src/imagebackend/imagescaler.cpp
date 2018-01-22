#include "imagescaler.h"
#include "config.h"

ImageScaler::ImageScaler()
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleResult()));
}

void ImageScaler::handleResult()
{
    buffer = watcher.result();
}

QImage ImageScaler::scale(QImage source)
{
    // if no image is available, do nothing
    if(source.width() * source.height() <= 2 || target_width == 0)
        return;

    TimeLoggerLog("%s", "Scaling image");

    QImage result = source;

    // scaling it if necessary
    if(target_width != source.width())
        result = source.scaledToWidth(target_width);

    return result;
}

void ImageScaler::setInput(QImage source)
{
    QFuture<QImage> future = QtConcurrent::run(*this, &ImageScaler::scale, source);
    watcher.setFuture(future);
}

ImageScaler::ImageScaler(int target_width)
{
    this->target_width = target_width;
}

QImage ImageScaler::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buffer;
}
