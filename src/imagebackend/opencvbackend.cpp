#include "opencvbackend.h"
#include "QtOpenCV/cvmatandqimage.h"
using namespace std;

OpenCVCameraBackend::OpenCVCameraBackend(int cam_id)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    // copy id to object property
    camera_id = cam_id;
}

QImage OpenCVCameraBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(!is_initialized)
        setupCV();

    // reading the image
    Mat mat;
    stream->read(mat);

    // converting the matrix to qimage and then to pixmap
    return QtOcv::mat2Image(mat);
}

QPixmap OpenCVCameraBackend::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(requestImage(id, size, requestedSize));
}

void OpenCVCameraBackend::setupCV() {
    // opening the camera
    stream = new VideoCapture(camera_id);

    // cannot continue on error
    // check if video device has been initialised
    if (!stream->isOpened()) {
        qFatal("cannot open camera");
    }

    is_initialized = true;
}
