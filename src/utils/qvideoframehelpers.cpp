#include "timelogger.h"
#include "qvideoframehelpers.h"
#include <QElapsedTimer>
#include <QList>
#include <QVideoFrame>
#include "yuv2rgb.h"

uchar QVideoFrameHelpers::rgb[MAX_SIZE];
uchar QVideoFrameHelpers::yuv1[MAX_SIZE];
uchar QVideoFrameHelpers::yuv2[MAX_SIZE];

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

void QVideoFrameHelpers::halfYUV(uchar* src, uchar* dst, int w, int h)
{
    //byte[] yuv = new byte[imageWidth/2 * imageHeight/2 * 3 / 2];
    // halve yuma
    int i = 0;
    for (int y = 0; y < h; y+=2) {
        for (int x = 0; x < w; x+=2) {
            dst[i] = src[y * w + x];
            i++;
        }
    }
    // halve U and V color components
    for (int y = 0; y < h / 2; y+=2) {
        for (int x = 0; x < w; x += 4) {
            dst[i] = src[(w * h) + (y * w) + x];
            i++;
            dst[i] = src[(w * h) + (y * w) + (x + 1)];
            i++;
        }
    }
}

QImage QVideoFrameHelpers::VideoFrameToImage(const QVideoFrame &frameOriginal)
{
    TimeLoggerThroughput("%s", "[ANALYZE] Begin FrameConvert");
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

    // .. and size
    int w = frameOriginal.width();
    int h = frameOriginal.height();

    // size /= 4
    halfYUV(img, yuv1, w, h);
    w /= 2;
    h /= 2;
    halfYUV(yuv1, yuv2, w, h);
    w /= 2;
    h /= 2;
    img = yuv2;

    // format of the resulting QImage
    QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());

    // QImage cannot work with NV21 (Android)
    // this call to yuv2rgb library converts it to RGB888
    if(frame.pixelFormat() == QVideoFrame::Format_NV21)
    {
        nv21_to_rgb(rgb, img, w, h);
        img = rgb;
        fmt = QImage::Format_RGB888;
    }

    // if format is still invalid, the application stops
    if(fmt == QImage::Format_Invalid)
    {
        qFatal("Cannot determine output format");
    }

    // the resulting QImage
    QImage image(img,
                 w,
                 h, fmt);

    // unmapping source from memory
    frame.unmap();

    QImage result = image.copy();

    TimeLoggerThroughput("%s", "[ANALYZE] End FrameConvert");

    return(result);
}

QImage QVideoFrameHelpers::empty()
{
    QImage image(1, 1, QImage::Format_RGB888);
    image.fill(QColor(0, 0, 0));
    return image;
}
