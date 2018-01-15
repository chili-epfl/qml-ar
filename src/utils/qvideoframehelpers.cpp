#include "qvideoframehelpers.h"
#include <QElapsedTimer>
#include <QList>
#include <QVideoFrame>
#include "yuv2rgb/yuv2rgb.h"
#include "config.h"

QList<QVideoFrame::PixelFormat> QVideoFrameHelpers::supportedPixelFormats()
{
    return QList<QVideoFrame::PixelFormat>()
            << QVideoFrame::Format_ARGB32
            << QVideoFrame::Format_ARGB32_Premultiplied
            << QVideoFrame::Format_RGB32
            << QVideoFrame::Format_RGB24
            << QVideoFrame::Format_RGB565
            << QVideoFrame::Format_RGB555
            << QVideoFrame::Format_ARGB8565_Premultiplied
            << QVideoFrame::Format_BGRA32
            << QVideoFrame::Format_BGRA32_Premultiplied
            << QVideoFrame::Format_BGR32
            << QVideoFrame::Format_BGR24
            << QVideoFrame::Format_BGR565
            << QVideoFrame::Format_BGR555
            << QVideoFrame::Format_BGRA5658_Premultiplied
            << QVideoFrame::Format_AYUV444
            << QVideoFrame::Format_AYUV444_Premultiplied
            << QVideoFrame::Format_YUV444
            << QVideoFrame::Format_YUV420P
            << QVideoFrame::Format_YV12
            << QVideoFrame::Format_UYVY
            << QVideoFrame::Format_YUYV
            << QVideoFrame::Format_NV12
            << QVideoFrame::Format_NV21
            << QVideoFrame::Format_IMC1
            << QVideoFrame::Format_IMC2
            << QVideoFrame::Format_IMC3
            << QVideoFrame::Format_IMC4
            << QVideoFrame::Format_Y8
            << QVideoFrame::Format_Y16
            << QVideoFrame::Format_Jpeg
            << QVideoFrame::Format_CameraRaw
            << QVideoFrame::Format_AdobeDng;
}

QImage QVideoFrameHelpers::VideoFrameToImage(const QVideoFrame &frameOriginal)
{
    // buffer for nv21 -> rgb conversion
    static uchar* rgb = new uchar[MAX_SIZE];

    Q_ASSERT(MAX_SIZE >= frameOriginal.width() * frameOriginal.height() * 3);

    // do nothing if no image found
    if(frameOriginal.width() * frameOriginal.height() == 0) return QImage();

    Q_ASSERT(frameOriginal.width() <= MAX_IMG_SIDE);
    Q_ASSERT(frameOriginal.height() <= MAX_IMG_SIDE);

    // mapping frame to memory
    QVideoFrame frame(frameOriginal);
    frame.map(QAbstractVideoBuffer::ReadOnly);

    // bits of the image as byte array
    uchar* img = (uchar*) frame.bits();

    // set this to true to delete img at the end
    bool need_delete_img = false;

    // format of the resulting QImage
    QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    // QImage cannot work with NV21 (Android)
    // this call to yuv2rgb library converts it to RGB888
    if(frame.pixelFormat() == QVideoFrame::Format_NV21)
    {
        nv21_to_rgb(rgb, img, frame.width(), frame.height());
        img = rgb;
        fmt = QImage::Format_RGB888;
        //need_delete_img = true;
    }

    // if format is still invalid, the application stops
    if(fmt == QImage::Format_Invalid)
    {
        qFatal("Cannot determine output format");
    }

    // the resulting QImage
    QImage image(img,
                 frame.width(),
                 frame.height(), fmt);

    // unmapping source from memory
    frame.unmap();

    // deleting img if it was created in this context
    // e.g. if NV21 format was used
    if(need_delete_img)
        delete img;

    TimeLoggerProfile("%s", "Converted image to RGB");

    return(image);
}
