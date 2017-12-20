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
    // maximum size for nv21_to_rgb buffer
    static const int MAX_SIZE = 4000000;

public:
    // return a list with all possible formats
    static QList<QVideoFrame::PixelFormat> supportedPixelFormats();

    // convert QVideoFrame to QImage
    // supports Android nv21 format (uses yuv2rgb library)
    static QImage VideoFrameToImage(const QVideoFrame &frameOriginal);
};

#endif // QVIDEOFRAMEHELPERS_H
