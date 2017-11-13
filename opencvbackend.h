#ifndef BACKEND_H
#define BACKEND_H

#include <QQuickImageProvider>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

class BackEnd : public QQuickImageProvider
{
public:
    BackEnd();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    void setupCV();

private:
    bool is_initialized = false;
    QPixmap buf;
    VideoCapture* stream1 = NULL;
};

#endif // BACKEND_H
