/**
 * @file markerbackend.h
 * @brief A backend for QML for marker detection preview
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include "imageproviderasync.h"
#include "markerdetector.h"
#include <QLinkedList>

/**
 * @brief A backend for QML for marker detection preview
 *
 * this class is a decorator on top of the camera
 * with a marker detector as a parameter
 */

class MarkerBackEnd : public ImageProviderAsync
{ Q_OBJECT
public:
    /**
    * @brief Set to true to delay images by latency
    */
    MarkerBackEnd(bool do_delay = true);

    /**
    * @brief Obtain processed image
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~MarkerBackEnd();

public slots:
    /**
    * @brief Slots to set data
    */
    void setCamera(PipelineContainer<QImage> cam);
    void setPreview(PipelineContainer<QImage> prev);
    void setDelay(int delay);

private:
    /**
    * @brief Buffers for preview image and raw image
    */
    QImage preview, camera;

    /**
    * @brief Storing this amount of frames
    */
    static const int MAX_BUFFER_SIZE = 10;

    /**
    * @brief Stored camera images
    */
    QLinkedList<QImage> camera_buffer;

    /**
    * @brief Delay for camera
    */
    int delay_in_frames;

    /**
    * @brief Use latency delay
    */
    bool do_delay;
};

#endif // UCHIYABACKEND_H
