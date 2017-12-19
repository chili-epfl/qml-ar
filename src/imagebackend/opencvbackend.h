#ifndef CV_BACKEND_H
#define CV_BACKEND_H

#include <QQuickImageProvider>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

/*
 * OpenCV camera backend for QML
 * Uses cv::VideoCapture
 */

class OpenCVCameraBackend : public QQuickImageProvider
{
public:
    // initialize with camera_id
    OpenCVCameraBackend(int cam_id = 0);

    // get image from the camera
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

private:
    // open the camera
    void setupCV();

    // id of the camera in VideoCapture(...)
    int camera_id;

    // camera opened?
    bool is_initialized = false;

    // buffer for the image
    QPixmap buf;

    // CV VideoCapture
    VideoCapture* stream = NULL;
};

#endif // CV_BACKEND_H
