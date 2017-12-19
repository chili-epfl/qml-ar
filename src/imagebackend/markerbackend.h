#ifndef UCHIYABACKEND_H
#define UCHIYABACKEND_H

#include <QMatrix4x4>

#include "qtbackend.h"
#include "markerdetector.h"

/*
 * A backend for QML for marker detection preview
 * this class is a decorator on top of the camera
 * with a marker detector as a parameter
 *
 */

class MarkerBackEnd : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    // decorate camera and marker detector object
    MarkerBackEnd(QQuickImageProvider* image_provider, MarkerDetector* marker_detector);

    // obtain processed image
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~MarkerBackEnd();
private:
    // marker detection pipeline
    MarkerDetector* detector;

    // image provider
    QQuickImageProvider* provider;
};

#endif // UCHIYABACKEND_H
