#include "opencvbackend.h"
#include "QtOpenCV/cvmatandqimage.h"
using namespace std;

OpenCVCameraBackEnd::OpenCVCameraBackEnd(int cam_id)
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    // copy id to object property
    camera_id = cam_id;
}

QPixmap OpenCVCameraBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(!is_initialized)
        setupCV();

    // reading the image
    Mat mat;
    stream->read(mat);

    // converting the matrix to qimage and then to pixmap
    QPixmap pm = QPixmap::fromImage(QtOcv::mat2Image(mat));

    return(pm);
}

void OpenCVCameraBackEnd::setupCV() {
    // opening the camera
    stream = new VideoCapture(camera_id);

    // cannot continue on error
    // check if video device has been initialised
    if (!stream->isOpened()) {
        qFatal("cannot open camera");
    }

    is_initialized = true;
}
