#include "markerbackend.h"
#include "qvideoframehelpers.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "QtOpenCV/cvmatandqimage.h"
#include "config.h"

using namespace cv;

MarkerBackEnd::MarkerBackEnd() : ImageProviderAsync()
{
    detector = NULL;
    camera = NULL;
}

void MarkerBackEnd::initialize(MarkerDetector* marker_detector)
{
    detector = marker_detector;
}

QImage MarkerBackEnd::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(id == "markers")
    {
        if(detector == NULL)
            return QVideoFrameHelpers::empty();
        QImage result = detector->getPreview();
        if(result.width() * result.height() == 0)
            return QVideoFrameHelpers::empty();
        return result;
    }
    else if(id == "camera")
    {
        if(camera == NULL)
            return QVideoFrameHelpers::empty();
        else return camera->requestImage("raw", size, requestedSize);
    }

    TimeLoggerLog("%s", "Invalid request id");
    return QVideoFrameHelpers::empty();
}

MarkerBackEnd::~MarkerBackEnd()
{
}

void MarkerBackEnd::setCameraBackend(QQuickImageProvider *provider)
{
    camera = provider;
    connect(camera, SIGNAL(imageAvailable(QImage)), this, SIGNAL(imageAvailable(QImage)));
}
