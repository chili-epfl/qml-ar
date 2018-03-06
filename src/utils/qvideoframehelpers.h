#ifndef QVIDEOFRAMEHELPERS_H
#define QVIDEOFRAMEHELPERS_H

#include <QList>
#include <QVideoFrame>
#include <QOpenGLContext>

/*
 * Helpers for QVideoFrame used in this project
 */

class TextureBuffer : public QAbstractVideoBuffer
{
public:
    TextureBuffer(uint id) : QAbstractVideoBuffer(GLTextureHandle), m_id(id) { }
    MapMode mapMode() const { return NotMapped; }
    uchar *map(MapMode, int *, int *) { return 0; }
    void unmap() { }
    QVariant handle() const { return QVariant::fromValue<GLuint>(m_id); }

private:
    GLuint m_id;
};

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

    // buffer for halving image two times
    static uchar yuv1[MAX_SIZE], yuv2[MAX_SIZE];

public:
    // return a list with all possible formats
    static QList<QVideoFrame::PixelFormat> supportedPixelFormats();

    // clone android frame
    static QVideoFrame cloneAndroidFrame(QVideoFrame* input);

    // convert QVideoFrame to QImage
    // supports Android nv21 format (uses yuv2rgb library)
    static QImage VideoFrameToImage(const QVideoFrame &frameOriginal);

    // returns empty black image
    static QImage empty();

    // downscale YUV image by half
    // https://stackoverflow.com/questions/17187193/resize-downsize-yuv420sp-image
    static void halfYUV(uchar *src, uchar *dst, int w, int h);
};

#endif // QVIDEOFRAMEHELPERS_H
