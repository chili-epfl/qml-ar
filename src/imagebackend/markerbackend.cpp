#include "markerbackend.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "QtOpenCV/cvmatandqimage.h"
#include "config.h"

using namespace cv;

MarkerBackEnd::MarkerBackEnd() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    detector = NULL;
    camera = NULL;
}

void MarkerBackEnd::initialize(MarkerDetector* marker_detector)
{
    detector = marker_detector;
}

QPixmap MarkerBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(id == "markers")
    {
        if(detector == NULL)
            return QPixmap(1, 1);
        QImage result = detector->getPreview();
        if(result.width() * result.height() == 0)
            return QPixmap(1, 1);
        return QPixmap::fromImage(result);
    }
    else if(id == "camera")
    {
        if(camera == NULL)
            return QPixmap(1, 1);
        else return camera->requestPixmap("raw", size, requestedSize);
    }

    TimeLoggerLog("%s", "Invalid request id");
    return QPixmap(1, 1);
}

MarkerBackEnd::~MarkerBackEnd()
{
}

void MarkerBackEnd::setCameraBackend(QQuickImageProvider *provider)
{
    camera = provider;
}
