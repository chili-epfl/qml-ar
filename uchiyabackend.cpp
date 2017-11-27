#include "uchiyabackend.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "QtOpenCV/cvmatandqimage.h"

using namespace cv;

UchiyaBackEnd::UchiyaBackEnd(QQuickImageProvider *image_provider) : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    provider = image_provider;

    // marker detection init will be done on first valid image request
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
    cvCopy(&src2mat2ipl, md->getSrcPtr());

    // run Uchiya pipeline
    md->process();

    // obtaining Uchiya image dst and returning it
    IplImage dst = *(md->getDstPtr());
    cv::Mat dst2mat = cv::cvarrToMat(&dst, false); // second parameter disables data copying
    QImage dst2mat2qimg = QtOcv::mat2Image(dst2mat);

    emit newImage();

    // default value
    return dst2mat2qimg;
}

QPixmap UchiyaBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id)
    // obtain input image from camera
    QPixmap input = provider->requestPixmap("raw", size, requestedSize);

    // process image and return the result
    return QPixmap::fromImage(processUchiya(input.toImage()));
}

UchiyaBackEnd::~UchiyaBackEnd()
{
    if(md != NULL) delete md;
}

QMatrix4x4 UchiyaBackEnd::getProjectionMatrix()
{
    QMatrix4x4 dst;
    if(md == NULL || !md->isHValid) return dst;
    MyMat src = md->H;
    dst.data()[0] = src(0,0);
    dst.data()[1] = src(1,0);
    dst.data()[2] = 0.0f;
    dst.data()[3] = src(2,0);

    dst.data()[4] = src(0,1);
    dst.data()[5] = src(1,1);
    dst.data()[6] = 0.0f;
    dst.data()[7] = src(2,1);

    dst.data()[8] = 0.0f;
    dst.data()[9] = 0.0f;
    dst.data()[10] = 1.0f;
    dst.data()[11] = 0.0f;

    dst.data()[12] = src(0,2);
    dst.data()[13] = src(1,2);
    dst.data()[14] = 0.0f;
    dst.data()[15] = src(2,2);

    QMatrix4x4 res = md->getOrthoMatrix() * dst;

    qDebug() << "getproj" << res;

    return res;
}
