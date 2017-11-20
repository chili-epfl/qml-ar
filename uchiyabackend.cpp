#include "uchiyabackend.h"

UchiyaBackEnd::UchiyaBackEnd() : QtCameraBackEnd()
{
    md = new UchiyaMarkerDetection(camera->viewfinderSettings().resolution().height(), camera->viewfinderSettings().resolution().width());
}

QImage UchiyaBackEnd::processUchiya(QImage temp)
{
    cv::Mat mat(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    IplImage ipltemp = mat;
    cvCopy(&ipltemp, (IplImage*) md->getimg());
    return temp;
}

QPixmap UchiyaBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(processUchiya(buf));
}

UchiyaBackEnd::~UchiyaBackEnd()
{
    delete md;
}
