#ifndef QVIDEOFRAMEHELPERS_H
#define QVIDEOFRAMEHELPERS_H

#include <QList>
#include <QVideoFrame>

/*
 * Helpers for QVideoFrame used in this project
 */

class QVideoFrameHelpers
{
private:
    // maximum image side length
    static const int MAX_IMG_SIDE = 2000;

    // maximum size for nv21_to_rgb buffer (w x h x 3 channels)
    // see yuv2rgb.h:68
    static const int MAX_SIZE = MAX_IMG_SIDE * MAX_IMG_SIDE * 3;

    // buffer for nv21 -> rgb conversion
    static uchar rgb[MAX_SIZE];

public:
    // return a list with all possible formats
    static QList<QVideoFrame::PixelFormat> supportedPixelFormats();

    // convert QVideoFrame to QImage
    // supports Android nv21 format (uses yuv2rgb library)
    static QImage VideoFrameToImage(const QVideoFrame &frameOriginal);
};

#endif // QVIDEOFRAMEHELPERS_H
