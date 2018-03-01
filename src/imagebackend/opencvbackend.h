#ifndef CV_BACKEND_H
#define CV_BACKEND_H

#include <QtConcurrent>
#include <QQuickImageProvider>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "imageproviderasync.h"

/*
 * OpenCV camera backend for QML
 * Uses cv::VideoCapture
 */

class OpenCVCameraBackend : public ImageProviderAsync
{ Q_OBJECT
public:
    // empty constructor
    OpenCVCameraBackend();

    virtual ~OpenCVCameraBackend();

    // copy constructor
    OpenCVCameraBackend(const OpenCVCameraBackend &backend);

    // initialize with camera_id
    OpenCVCameraBackend(int cam_id = 0);

    // get image from the camera
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    // request new image from camera
    void request();

    // obtain image from OpenCV
    QImage getImage();
private:
    // open the camera
    void setupCV();

    // id of the camera in VideoCapture(...)
    int camera_id;

    // camera opened?
    bool is_initialized = false;

    // buffer for the image
    QImage buf;

    // CV VideoCapture
    cv::VideoCapture* stream = NULL;

    // thread worker
    QFutureWatcher<QImage> watcher;

    // last image identity
    int image_id;

public slots:
    // get result from future
    void handleFinished();
};

#endif // CV_BACKEND_H
