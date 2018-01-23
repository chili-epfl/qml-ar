#include "opencvbackend.h"
#include "qvideoframehelpers.h"
#include "QtOpenCV/cvmatandqimage.h"
#include "config.h"

OpenCVCameraBackend::OpenCVCameraBackend(const OpenCVCameraBackend& backend) : ImageProviderAsync()
{
    this->camera_id = backend.camera_id;
    this->stream = backend.stream;
}

OpenCVCameraBackend::OpenCVCameraBackend() : ImageProviderAsync()
{

}

OpenCVCameraBackend::~OpenCVCameraBackend()
{
}

OpenCVCameraBackend::OpenCVCameraBackend(int cam_id) : ImageProviderAsync()
{
    // copy id to object property
    camera_id = cam_id;
    buf = QVideoFrameHelpers::empty();
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
    setupCV();
    request();
}

void OpenCVCameraBackend::request()
{
    // request new image
    QFuture<QImage> future = QtConcurrent::run(*this, &OpenCVCameraBackend::getImage);
    watcher.setFuture(future);
}

void OpenCVCameraBackend::handleFinished()
{
    // obtain previous result
    buf = watcher.result();

    emit imageAvailable(buf);

    TimeLoggerLog("%s", "Obtained CV image");

    // request new frame if this call was successful
    if(buf.width() * buf.height() > 1)
        request();
}

QImage OpenCVCameraBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    return buf.copy();
}

QImage OpenCVCameraBackend::getImage()
{
    // reading the image
    cv::Mat mat;
    stream->read(mat);

    // converting the matrix to qimage and then to pixmap
    return QtOcv::mat2Image(mat);
}

void OpenCVCameraBackend::setupCV() {
    // opening the camera
    stream = new cv::VideoCapture(camera_id);

    // cannot continue on error
    // check if video device has been initialised
    if (!stream->isOpened()) {
        qFatal("cannot open camera");
    }
}
