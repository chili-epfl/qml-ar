#ifndef IMAGEBACKEND_H
#define IMAGEBACKEND_H

#include "imageproviderasync.h"
#include <QFile>
#include <QImage>

/*
 * This class implements a QQuickImageProvider
 * showing a constant image from a file
 */

class ImageBackend : public ImageProviderAsync
{
private:
    QImage buffer;
public:
    // load image from the file
    ImageBackend(QString filename);

    // get image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEBACKEND_H
