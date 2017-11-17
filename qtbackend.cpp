#include "qtbackend.h"
#include <QtMultimedia/QCameraInfo>

QtCameraBackEnd::QtCameraBackEnd() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    buf = QImage();
    QCamera* camera = new QCamera(QCameraInfo::availableCameras().at(0));
    frameGrabber = new CameraFrameGrabber();
    camera->setViewfinder(frameGrabber);
    connect(frameGrabber, SIGNAL(frameAvailable(QImage)), this, SLOT(handleFrame(QImage)));
    camera->start();
}

QPixmap QtCameraBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(buf);
}

void QtCameraBackEnd::handleFrame(QImage img)
{
    buf = img.copy();
}
