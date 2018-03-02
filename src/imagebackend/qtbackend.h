#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QString>
#include <QtConcurrent>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"
#include "imageproviderasync.h"

/*
 *  QCamera backend for QML
 *  works on Linux and Android
 */

class QtCameraBackend : public ImageProviderAsync
{ Q_OBJECT

public:
    // cam_id indicates the camera index in QCameraInfo::availableCameras()
    QtCameraBackend(int cam_id = 0);

    // callback for main thread
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackend();

    // convert frame -> image
    void convert(QVideoFrame *frame, QImage *image);

    // return underlying camera object
    QCamera* getCamera();

    // start camera
    void start();
protected:
    // last image id
    int image_id;

    // information about current image
    PipelineContainerInfo image_info;

    // is frame available?
    bool frame_available = 0;

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

    // thread result
    QFutureWatcher<QImage> watcher;

public slots:
    // set this image as buffer
    void processQImage(QImage img);

    // process frame in thread
    void processQVideoFrame(const QVideoFrame &frame);

    // called on thread finish
    void handleFinished();
};

#endif // QTBACKEND_H
