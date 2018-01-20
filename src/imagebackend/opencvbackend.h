#ifndef CV_BACKEND_H
#define CV_BACKEND_H

#include "abstractcamerabackend.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

/*
 * OpenCV camera backend for QML
 * Uses cv::VideoCapture
 */

class OpenCVCameraBackend : public AbstractCameraBackend
{
public:
    // initialize with camera_id
    OpenCVCameraBackend(int cam_id = 0);

    void threadIteration(PipelineElement *input, PipelineElement *output);
private:
    // open the camera
    void setupCV();

    // id of the camera in VideoCapture(...)
    int camera_id;

    // camera opened?
    bool is_initialized = false;

    // CV VideoCapture
    VideoCapture* stream = NULL;
};

#endif // CV_BACKEND_H
