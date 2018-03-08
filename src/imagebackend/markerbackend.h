#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include "imageproviderasync.h"
#include "markerdetector.h"
#include <QLinkedList>

/*
 * A backend for QML for marker detection preview
 * this class is a decorator on top of the camera
 * with a marker detector as a parameter
 */

class MarkerBackEnd : public ImageProviderAsync
{ Q_OBJECT
public:
    // set to true to delay images by latency
    MarkerBackEnd(bool do_delay = true);

    // obtain processed image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~MarkerBackEnd();

public slots:
    // slots to set data
    void setCamera(PipelineContainer<QImage> cam);
    void setPreview(PipelineContainer<QImage> prev);
    void setDelay(int delay);

private:
    // buffers for preview image and raw image
    QImage preview, camera;

    // storing this amount of frames
    static const int MAX_BUFFER_SIZE = 10;

    // stored camera images
    QLinkedList<QImage> camera_buffer;

    // delay for camera
    int delay_in_frames;

    // use latency delay
    bool do_delay;
};

#endif // UCHIYABACKEND_H
