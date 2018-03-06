#include "timelogger.h"
#include "markerbackend.h"
#include "qvideoframehelpers.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvmatandqimage.h"

using namespace cv;

const int buf_size = 100;
QImage cam_buf[buf_size];
int cam_buf_idx = 0;

QImage get_from_buf(int delay = 0)
{
    int get_index = cam_buf_idx - 1 - delay;
    if(get_index < 0)
        get_index += buf_size;
    return cam_buf[get_index];
}

void add_to_buf(QImage i)
{
    cam_buf[cam_buf_idx++] = i;
    if(cam_buf_idx >= buf_size)
        cam_buf_idx = 0;
}

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
        //return camera;
        return get_from_buf(1);

    TimeLoggerLog("%s", "Invalid request id");
    return QVideoFrameHelpers::empty();
}

MarkerBackEnd::~MarkerBackEnd()
{
}

void MarkerBackEnd::setCamera(PipelineContainer<QImage> cam)
{
    //camera = cam;
    add_to_buf(cam.o());
}

void MarkerBackEnd::setPreview(PipelineContainer<QImage> prev)
{
    preview = prev;
}
