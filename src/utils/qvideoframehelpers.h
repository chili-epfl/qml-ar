/**
 * @file qvideoframehelpers.h
 * @brief Helpers for QVideoFrame used in this project
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef QVIDEOFRAMEHELPERS_H
#define QVIDEOFRAMEHELPERS_H

#include <QList>
#include <QVideoFrame>
#include <QOpenGLContext>

/**
 * @brief Qt interface for a texture
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


/**
 * @brief Helpers for QVideoFrame used in this project
 */

class QVideoFrameHelpers
{
private:
    /**
    * @brief Maximum image side length
    */
    static const int MAX_IMG_SIDE = 2000;

    /**
    * @brief Maximum size for nv21_to_rgb buffer (w x h x 3 channels)
    * See yuv2rgb.h:68
    */
    static const int MAX_SIZE = MAX_IMG_SIDE * MAX_IMG_SIDE * 3;

    /**
    * @brief Buffer for nv21 -> rgb conversion
    */
    static uchar rgb[MAX_SIZE];

    /**
    * @brief Buffer for halving image two times
    */
    static uchar yuv1[MAX_SIZE], yuv2[MAX_SIZE];

public:
    /**
    * @brief Return a list with all possible formats
    */
    static QList<QVideoFrame::PixelFormat> supportedPixelFormats();

    /**
    * @brief Clone android frame
    */
    // see see https://bugreports.qt.io/browse/QTBUG-48567
    /**
    * @brief Response by Christoph Keller, 29 Apr '17 12:57 PM
    */
    static QVideoFrame cloneAndroidFrame(QVideoFrame* input);

    /**
    * @brief Convert QVideoFrame to QImage
    * Supports Android nv21 format (uses yuv2rgb library)
    */
    static QImage VideoFrameToImage(const QVideoFrame &frameOriginal);

    /**
    * @brief Returns empty black image
    */
    static QImage empty();

    /**
    * @brief Downscale YUV image by half
    */
    // https://stackoverflow.com/questions/17187193/resize-downsize-yuv420sp-image
    static void halfYUV(uchar *src, uchar *dst, int w, int h);

    static QImage VideoFrameBinToImage(const QVideoFrame &frameOriginal);
};

#endif // QVIDEOFRAMEHELPERS_H
