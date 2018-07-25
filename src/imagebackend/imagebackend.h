/**
 * @file imagebackend.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

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
    /**
    * @brief Load image from the file
    */
    ImageBackend(QString filename);

    /**
    * @brief Get image
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // IMAGEBACKEND_H
