#include <iostream>
#include "opencvbackend.h"
using namespace std;

BackEnd::BackEnd()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap BackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(!is_initialized)
        setupCV();

    Mat mat;
    stream1->read(mat);

    QPixmap pm = QPixmap::fromImage(QImage((unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888));
    return(pm);
}

void BackEnd::setupCV() {
    stream1 = new VideoCapture(0);   //0 is the id of video device.0 if you have only one camera.

    if (!stream1->isOpened()) { //check if video device has been initialised
        throw "cannot open camera";
    }

    is_initialized = true;
}
