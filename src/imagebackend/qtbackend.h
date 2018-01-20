#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QString>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"
#include "threadworker.h"
#include "pipelinethread.h"
#include "abstractcamerabackend.h"

/*
 *  QCamera backend for QML
 *  works on Linux and Android
 */

class QtCameraBackend : public AbstractCameraBackend
{ Q_OBJECT

public:
    // cam_id indicates the camera index in QCameraInfo::availableCameras()
    QtCameraBackend(int cam_id);

    // convert frame -> image
    void threadIteration(PipelineElement *frame, PipelineElement *image);
protected:
    PipelineElement last_frame;

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
