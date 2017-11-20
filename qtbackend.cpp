#include "qtbackend.h"
#include <QtMultimedia/QCameraInfo>
#include <QVideoProbe>
#include <QException>
#include <QElapsedTimer>
#include "voidviewfinder.h"

QtCameraBackEnd::QtCameraBackEnd(int cam_id) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    timer.start();
    buf = QImage();
    qDebug() << "Number of cameras:" << QCameraInfo::availableCameras().size();
    camera = new QCamera(QCameraInfo::availableCameras().at(cam_id));

#ifdef Q_OS_ANDROID
    camera->setCaptureMode(QCamera::CaptureStillImage);
    viewfinder = new VoidViewFinder;
    camera->setViewfinder(viewfinder);
    probe = new QVideoProbe(this);
    if(probe->setSource((QMediaObject *) camera))
        connect(probe, SIGNAL(videoFrameProbed(const QVideoFrame &)), this, SLOT(handleFrameFrame(const QVideoFrame &)));
    else
        qFatal("Can't connect probe to camera");
#elif defined Q_OS_LINUX
    frameGrabber = new CameraFrameGrabber();
    camera->setViewfinder(frameGrabber);
    camera->viewfinderSettings().setMaximumFrameRate(0);
    camera->viewfinderSettings().setMinimumFrameRate(0);
    connect(frameGrabber, SIGNAL(frameAvailable(QImage)), this, SLOT(handleFrame(QImage)));
#else
    #error "OS must be either Android or Linux, camera is unsupported on other systems"
#endif

    camera->start();
}

QPixmap QtCameraBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(id == "raw")
        return QPixmap::fromImage(buf);

    qFatal("Invalid pixmap id");

    return QPixmap();
}

QtCameraBackEnd::~QtCameraBackEnd() {
    delete camera;
    delete frameGrabber;
}

void QtCameraBackEnd::handleFrame(QImage img)
{
    buf = img;
}

void QtCameraBackEnd::handleFrameFrame(const QVideoFrame &frame)
{
    qDebug() << "Elapsed " << timer.elapsed();
    if(timer.elapsed() > 20)
    {
        qDebug() << "Updating image";
        buf = CameraFrameGrabber::VideoFrameToImage(frame).copy();
        timer.start();
    }
}
