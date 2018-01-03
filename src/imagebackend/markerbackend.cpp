#include "markerbackend.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "QtOpenCV/cvmatandqimage.h"

using namespace cv;

MarkerBackEnd::MarkerBackEnd() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    detector = NULL;
}

void MarkerBackEnd::initialize(MarkerDetector* marker_detector)
{
    detector = marker_detector;
}

QPixmap MarkerBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(detector == NULL)
        return QPixmap();
    return QPixmap::fromImage(detector->getPreview());
}

MarkerBackEnd::~MarkerBackEnd()
{
}
