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

    // decorate camera and marker detector object
    void initialize(MarkerDetector* marker_detector);

    // obtain processed image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~MarkerBackEnd();

    // set raw camera
    void setCameraBackend(QQuickImageProvider* provider);
private:
    // marker detection pipeline
    MarkerDetector* detector;

    // camera provider
    QQuickImageProvider* camera;
};

#endif // UCHIYABACKEND_H
