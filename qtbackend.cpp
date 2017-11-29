#include "qtbackend.h"
#include <QtMultimedia/QCameraInfo>
#include <QVideoProbe>
#include <QException>
#include <QElapsedTimer>
#include "qvideoframehelpers.h"
#include "voidviewfinder.h"

QtCameraBackend::QtCameraBackend(int cam_id) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    // starting dirty hack timer
    timer.start();

    // initial buffer value is an empty image
    buf = QImage();

    // initializing camera
    qDebug() << "Number of cameras:" << QCameraInfo::availableCameras().size();
    camera = new QCamera(QCameraInfo::availableCameras().at(cam_id));

    // installing camera callback
    // has different implementations for Android/Linux
    // since Android camera is poorly supported in Android NDS (Nov'17)
#ifdef Q_OS_ANDROID
    // setting up a viewfinder which does nothing
    viewfinder = new VoidViewFinder;
    camera->setViewfinder(viewfinder);

    // this object does image grabbing
    probe = new QVideoProbe(this);

    // installing callback
    if(probe->setSource((QMediaObject *) camera))
        connect(probe, SIGNAL(videoFrameProbed(const QVideoFrame &)), this, SLOT(processQVideoFrame(const QVideoFrame &)));
    else
        qFatal("Can't connect probe to camera");

#elif defined Q_OS_LINUX
    // using custom class as a viewfinder
    frameGrabber = new CameraFrameGrabber();
    camera->setViewfinder(frameGrabber);

    // this option is required for the Logitech webcam to work
    camera->viewfinderSettings().setMaximumFrameRate(0);
    camera->viewfinderSettings().setMinimumFrameRate(0);

    // installing callback
    connect(frameGrabber, SIGNAL(frameAvailable(QImage)), this, SLOT(processQImage(QImage)));
#else
    #error "OS must be either Android or Linux, camera is unsupported on other systems"
#endif

    // starting camera thread
    camera->start();
}

QPixmap QtCameraBackend::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(id == "raw")
        return QPixmap::fromImage(buf);

    qFatal("Invalid pixmap id");

    return QPixmap();
}

QtCameraBackend::~QtCameraBackend() {
    delete camera;
    delete frameGrabber;
}

void QtCameraBackend::processQImage(QImage img)
{
    //qDebug() << "Updating image";
    buf = img;
}

void QtCameraBackend::processQVideoFrame(const QVideoFrame &frame)
{
    //qDebug() << "Elapsed " << timer.elapsed();

    // updating the buffer if enough time has passed
    if(timer.elapsed() > update_ms)
    {
        processQImage(QVideoFrameHelpers::VideoFrameToImage(frame).copy());
        timer.start();
    }
}
