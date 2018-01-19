#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QQuickImageProvider>
#include <QString>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"
#include "threadexecutor.h"

/*
 *  QCamera backend for QML
 *  works on Linux and Android
 */

class QtCameraBackend : public QObject, public QQuickImageProvider
{ Q_OBJECT

public:
    ThreadExecutor<QVideoFrame, QImage, QtCameraBackend>* converter;

    // cam_id indicates the camera index in QCameraInfo::availableCameras()
    QtCameraBackend(int cam_id = 0);

    // initialize from an existing QCamera
    QtCameraBackend(QCamera* cam);

    // callback for main thread
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackend();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    // convert frame -> image
    void convert(QVideoFrame *frame, QImage *image);
protected:
    QVideoFrame last_frame;

    // image buffer
    // is written in processQImage slot
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

    // initialize the object after setting camera ptr
    void init();

    // if set to true, will install a void viewfinder
    bool need_viewfinder;

public slots:
    void processQImage(QImage img);
    void processQVideoFrame(const QVideoFrame &frame);
};

#endif // QTBACKEND_H
