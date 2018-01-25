#ifndef RANDOMIMAGEBACKEND_H
#define RANDOMIMAGEBACKEND_H

#include "imageproviderasync.h"
#include <QImage>
#include <QTimer>

/*
 * This class takes input from pre-created
 * byte array and outputs it as a QImage
 */

class RandomImageBackend : public ImageProviderAsync
{ Q_OBJECT
private:
    // pointer to data
    uchar* data;

    // timer to update
    QTimer t;

    // maximum number of images
    const int max_images = 100;

    // width and height parameters
    int w, h;

    // image to return
    int current_image;

public:
    RandomImageBackend(int w = 640, int h = 480, int dt_ms = 10);

public slots:

    // generate and emit new image
    void updateImage();
};

#endif // RANDOMIMAGEBACKEND_H
