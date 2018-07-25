/**
 * @file markerbackend.cpp
 * @brief A backend for QML for marker detection preview
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "markerbackend.h"
#include "qvideoframehelpers.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvmatandqimage.h"
#include "qlinkedlistat.h"
#include <QtMath>

using namespace cv;

MarkerBackEnd::MarkerBackEnd(bool do_delay) : ImageProviderAsync()
{
    this->do_delay = do_delay;
    preview = QVideoFrameHelpers::empty();
    camera = QVideoFrameHelpers::empty();
}

QImage MarkerBackEnd::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(size) Q_UNUSED(requestedSize)
    if(id == "markers")
        return preview;
    else if(id == "camera")
    {
        if(!do_delay) return camera;
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
}
