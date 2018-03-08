#include "timelogger.h"
#include "markerbackend.h"
#include "qvideoframehelpers.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvmatandqimage.h"
#include "qlinkedlistat.h"
#include <QtMath>

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
    {
        qDebug() << "delay" << delay_in_frames;
        if(camera_buffer.isEmpty()) return camera;
        return QLinkedListAt<QImage>(camera_buffer, delay_in_frames);
    }

    TimeLoggerLog("%s", "Invalid request id");
    return QVideoFrameHelpers::empty();
}

MarkerBackEnd::~MarkerBackEnd()
{
}

void MarkerBackEnd::setCamera(PipelineContainer<QImage> cam)
{
    camera = cam;
    camera_buffer.push_front(cam.o());
    if(camera_buffer.size() > MAX_BUFFER_SIZE)
        camera_buffer.pop_back();
}

void MarkerBackEnd::setPreview(PipelineContainer<QImage> prev)
{
    preview = prev;
}

void MarkerBackEnd::setDelay(int delay)
{
    delay_in_frames = delay;
    qDebug() << "setDelay" << delay;
}
