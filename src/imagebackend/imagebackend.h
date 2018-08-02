/**
 * @file imagebackend.h
 * @brief This class implements a QQuickImageProvider
 * showing a constant image from a file
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef IMAGEBACKEND_H
#define IMAGEBACKEND_H

#include "imageproviderasync.h"
#include <QFile>
#include <QImage>
#include <QTimer>

/**
 * @brief This class implements a QQuickImageProvider
 * showing a constant image from a file
 */

class ImageBackend : public ImageProviderAsync
{ Q_OBJECT
private:
    QImage buffer;
    QTimer timer;
    int image_id = 0;

public:
    /**
    * @brief Load image from the file
    */
    ImageBackend(QString filename = QString());

    virtual ~ImageBackend();

    /**
    * @brief Get image
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

public slots:
    /**
    * @brief Emit the imageAvailable signal
    */
    void update();
};

#endif // IMAGEBACKEND_H
