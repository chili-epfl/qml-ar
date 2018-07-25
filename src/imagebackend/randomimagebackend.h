/**
 * @file randomimagebackend.h
 * @brief This class takes input from pre-created
 * byte array and outputs it as a QImage
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef RANDOMIMAGEBACKEND_H
#define RANDOMIMAGEBACKEND_H

#include "imageproviderasync.h"
#include <QImage>
#include <QTimer>

/**
 * @brief This class takes input from pre-created
 * byte array and outputs it as a QImage
 */

class RandomImageBackend : public ImageProviderAsync
{ Q_OBJECT
private:
    /**
    * @brief Pointer to data
    */
    uchar* data;

    /**
    * @brief Timer to update
    */
    QTimer t;

    /**
    * @brief Maximum number of images
    */
    const int max_images = 100;

    /**
    * @brief Width and height parameters
    */
    int w, h;

    /**
    * @brief Image to return
    */
    int current_image;

public:
    RandomImageBackend(int w = 640, int h = 480, int dt_ms = 10);

public slots:

    /**
    * @brief Generate and emit new image
    */
    void updateImage();
};

#endif // RANDOMIMAGEBACKEND_H
