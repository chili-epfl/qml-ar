#ifndef IMAGEBACKEND_H
#define IMAGEBACKEND_H

#include <QQuickImageProvider>
#include <QFile>
#include <QImage>

/*
 * This class implements a QQuickImageProvider
 * showing a constant image from a file
 */

class ImageBackend : public QQuickImageProvider
{
private:
    QImage buffer;
public:
    // load image from the file
    ImageBackend(QString filename);

    // get image
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEBACKEND_H
