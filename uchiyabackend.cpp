#include "uchiyabackend.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv.h"
#include "QtOpenCV/cvmatandqimage.h"

using namespace cv;

UchiyaBackEnd::UchiyaBackEnd() : QtCameraBackEnd(0)
{
    md = NULL;
}

QImage UchiyaBackEnd::processUchiya(QImage src)
{
    // if the image is invalid, no need for marker detection
    // this happens at the start of the application
    if(src.height() * src.width() <= 0)
    {
        qDebug() << "Empty image";
        return src;
    }

    // initialize marker detection pipeline
    // when we know the camera image size
    if(md == NULL)
    {
        qDebug() << "Creating new UchiyaMarkerDetection";
        md = new UchiyaMarkerDetection(src.height(), src.width());
    }

    // putting camera src image to Uchiya pipeline
    cv::Mat src2mat = QtOcv::image2Mat(src, CV_8UC3);
    IplImage src2mat2ipl = src2mat;
    cvCopy(&src2mat2ipl, md->getimg());

    // run Uchiya pipeline
    md->process();

    // obtaining Uchiya image dst and returning it
    IplImage dst = *(md->getres());
    cv::Mat dst2mat = cv::cvarrToMat(&dst, false); // second parameter disables data copying
    QImage dst2mat2qimg = QtOcv::mat2Image(dst2mat);

    // default value
    return dst2mat2qimg;
}

QPixmap UchiyaBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(processUchiya(buf));
}

UchiyaBackEnd::~UchiyaBackEnd()
{
    delete md;
}
