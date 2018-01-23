#include "markerbackend.h"
#include "qvideoframehelpers.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "QtOpenCV/cvmatandqimage.h"
#include "config.h"

using namespace cv;

MarkerBackEnd::MarkerBackEnd() : ImageProviderAsync()
{
    preview = QVideoFrameHelpers::empty();
    camera = QVideoFrameHelpers::empty();
}

QImage MarkerBackEnd::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(id == "markers")
        return preview;
    else if(id == "camera")
        return camera;

    TimeLoggerLog("%s", "Invalid request id");
    return QVideoFrameHelpers::empty();
}

MarkerBackEnd::~MarkerBackEnd()
{
}

void MarkerBackEnd::setCamera(QImage cam)
{
    camera = cam;
}

void MarkerBackEnd::setPreview(QImage prev)
{
    preview = prev;
}
