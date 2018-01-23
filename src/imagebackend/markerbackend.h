#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include "imageproviderasync.h"
#include "markerdetector.h"

/*
 * A backend for QML for marker detection preview
 * this class is a decorator on top of the camera
 * with a marker detector as a parameter
 */

class MarkerBackEnd : public ImageProviderAsync
{ Q_OBJECT
public:
    MarkerBackEnd();

    // obtain processed image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~MarkerBackEnd();
public slots:
    // slots to set data
    void setCamera(QImage cam);
    void setPreview(QImage prev);
private:
    // buffers for preview image and raw image
    QImage preview, camera;
};

#endif // UCHIYABACKEND_H
