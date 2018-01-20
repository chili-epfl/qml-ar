#include "opencvbackend.h"
#include "QtOpenCV/cvmatandqimage.h"
#include "pipelinethread.h"
#include "threadworker.h"
using namespace std;

OpenCVCameraBackend::OpenCVCameraBackend(int cam_id)
{
    // copy id to object property
    camera_id = cam_id;

    is_initialized = false;

    pipelineThread = new PipelineThread(this);
}

void OpenCVCameraBackend::threadIteration(PipelineElement *input, PipelineElement *output)
{ Q_UNUSED(input);
    if(!is_initialized)
        setupCV();

    // reading the image
    Mat mat;
    stream->read(mat);

    // converting the matrix to qimage
    output->image = QtOcv::mat2Image(mat);

    pipelineThread->usleep(1000);

    pipelineThread->update();
}

void OpenCVCameraBackend::setupCV() {
    // opening the camera
    stream = new VideoCapture(camera_id);

    // cannot continue on error
    // check if video device has been initialised
    if (!stream->isOpened()) {
        qFatal("cannot open camera");
    }

    is_initialized = true;
}
