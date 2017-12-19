#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QQuickImageProvider>
#include <QString>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"

// QCamera backend for QML
// works on Linux and Android

class QtCameraBackend : public QObject, public QQuickImageProvider
{ Q_OBJECT

public:
    // cam_id indicates the camera index in QCameraInfo::availableCameras()
    QtCameraBackend(int cam_id = 0);

    // callback for main thread
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackend();

protected:
    // image buffer
    // is written in processQImage signal
    // is read in requestPixmap
    // TODO: thread safety (now can crash randomly)
    QImage buf;

    // camera object
    QCamera* camera;

// LINUX
    CameraFrameGrabber* frameGrabber;
//

// ANDROID
    // QVideoProbe for Android version
    QVideoProbe* probe;

    // void viewfinder for Android version
    VoidViewFinder* viewfinder;
//

    // timer is a hack for updating frames not-so-fast
    QElapsedTimer timer;

    // only updating image each update_ms
    static const int update_ms = 20;

public slots:
    void processQImage(QImage img);
    void processQVideoFrame(const QVideoFrame &frame);
};

#endif // QTBACKEND_H
