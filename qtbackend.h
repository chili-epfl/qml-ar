#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QQuickImageProvider>
#include <QString>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"

class QtCameraBackEnd : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    QtCameraBackEnd();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackEnd();

protected:
    QImage buf;
    QCamera* camera;
    CameraFrameGrabber* frameGrabber;
    QVideoProbe* probe;
    VoidViewFinder* viewfinder;
    QElapsedTimer timer;
public slots:
    void handleFrame(QImage img);
    void handleFrameFrame(const QVideoFrame &frame);
};

#endif // QTBACKEND_H
