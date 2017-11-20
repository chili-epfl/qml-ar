#include "uchiyabackend.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

UchiyaBackEnd::UchiyaBackEnd() : QtCameraBackEnd(0)
{
    md = NULL;
}

QImage UchiyaBackEnd::processUchiya(QImage src)
{
    if(src.height() > 0)
    {
        QImage temp = src.rgbSwapped();
        if(md == NULL)
            md = new UchiyaMarkerDetection(temp.width(), temp.height());
        cv::Mat mat(temp.width(),temp.height(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
        IplImage* src_ipl = new IplImage(mat);
        IplImage* dst_ipl = (IplImage*) md->getimg();


        QString s;
        QTextStream ss(&s);
        for(int i = 0; i < 10; i++)
            ss << (int) ((char*) src_ipl->imageData)[i] << " ";

        ss.flush();

        qDebug() << "img data " << s;

        cvCopy(src_ipl, dst_ipl);
        //md->process();

        cv::Mat res = cv::cvarrToMat(dst_ipl);
        qDebug() << dst_ipl->height << dst_ipl->width;

        namedWindow( "Display window", WINDOW_AUTOSIZE );
        imshow( "Display window", res );
        //return QImage(res.data, res.cols, res.rows, res.step, QImage::Format_RGB888);

    }
    return src;
}

QPixmap UchiyaBackEnd::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(processUchiya(buf));
}

UchiyaBackEnd::~UchiyaBackEnd()
{
    delete md;
}
