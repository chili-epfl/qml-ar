#include "qtbackend.h"
#include <QtMultimedia/QCameraInfo>
#include <QVideoProbe>
#include <QException>
#include "qvideoframehelpers.h"
#include "voidviewfinder.h"
#include "timelogger.h"
#include "config.h"

QtCameraBackend::QtCameraBackend(int cam_id) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    // initializing camera
    TimeLoggerLog("Number of cameras: %d", QCameraInfo::availableCameras().size());
    camera = new QCamera(QCameraInfo::availableCameras().at(cam_id));
    need_viewfinder = 1;
    frame_available = 0;
    init();
}

void QtCameraBackend::init()
{
    // initial buffer value is an empty image
    buf = QImage();

    // installing camera callback
    // has different implementations for Android/Linux
    // since Android camera is poorly supported in Android NDK (Nov'17)

    // android callback will be installed in start()

#if defined Q_OS_LINUX && !defined Q_OS_ANDROID && !defined QT_BACKEND_FORCE_VIDEOPROBE
    // using custom class as a viewfinder
    frameGrabber = new CameraFrameGrabber();
    camera->setViewfinder(frameGrabber);

    // installing callback
    connect(frameGrabber, SIGNAL(frameAvailable(QImage)), this, SLOT(processQImage(QImage)));
#endif
}

void QtCameraBackend::start()
{
#if defined Q_OS_ANDROID || defined QT_BACKEND_FORCE_VIDEOPROBE
    // setting up a viewfinder which does nothing
    //    if(need_viewfinder)
    //    {
    //        viewfinder = new VoidViewFinder;
    //        camera->setViewfinder(viewfinder);
    //    }

    // this object does image grabbing
    probe = new QVideoProbe(this);

    // installing callback
    if(probe->setSource((QMediaObject *) camera))
        connect(probe, SIGNAL(videoFrameProbed(const QVideoFrame &)), this, SLOT(processQVideoFrame(const QVideoFrame &)));
    else
        qFatal("Can't connect probe to camera");
#endif

    // starting camera thread
    camera->start();
}

QImage QtCameraBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buf;
}

QPixmap QtCameraBackend::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return QPixmap::fromImage(buf);
}

QtCameraBackend::~QtCameraBackend() {
    delete camera;
}

void QtCameraBackend::processQImage(QImage img)
{
    buf = img;
}

void QtCameraBackend::convert(QVideoFrame* frame, QImage* image)
{
    *image = QVideoFrameHelpers::VideoFrameToImage(*frame).copy();
}

QCamera *QtCameraBackend::getCamera()
{
    return camera;
}

void QtCameraBackend::processQVideoFrame(const QVideoFrame &frame)
{
    TimeLoggerProfile("%s", "Received image from camera");
    last_frame = frame;
    convert(&last_frame, &buf);
    frame_available = 1;
}
