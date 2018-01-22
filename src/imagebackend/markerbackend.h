#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include <QMatrix4x4>
#include "qtbackend.h"
#include "markerdetector.h"

/*
 * A backend for QML for marker detection preview
 * this class is a decorator on top of the camera
 * with a marker detector as a parameter
 */

class MarkerBackEnd : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    MarkerBackEnd();

    // decorate camera and marker detector object
    void initialize(MarkerDetector* marker_detector);

    // obtain processed image
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

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
