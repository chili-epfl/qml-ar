#ifndef CV_BACKEND_H
#define CV_BACKEND_H

#include <QQuickImageProvider>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class OpenCVCameraBackEnd : public QQuickImageProvider
{
public:
    OpenCVCameraBackEnd();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    void setupCV();

private:
    bool is_initialized = false;
    QPixmap buf;
    VideoCapture* stream1 = NULL;
};

#endif // CV_BACKEND_H
