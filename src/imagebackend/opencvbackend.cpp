#include "timelogger.h"
#include "opencvbackend.h"
#include "qvideoframehelpers.h"
#include "cvmatandqimage.h"

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
    watcher.setParent(this);
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
    setupCV();
    image_id = 0;
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

    // sending image
    emit imageAvailable(PipelineContainer<QImage>
                        (buf.copy(), PipelineContainerInfo(image_id).checkpointed("Camera")));

    // next image will have different id
    image_id++;

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
    TimeLoggerLog("%s", "[ANALYZE] Begin CVGetImage");
    // reading the image
    cv::Mat mat;
    stream->read(mat);

    // converting the matrix to qimage and then to pixmap
    QImage result = QtOcv::mat2Image(mat);
    TimeLoggerLog("%s", "[ANALYZE] End CVGetImage");
    return result;
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
