#include "cameraframegrabber.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

CameraFrameGrabber::CameraFrameGrabber(QObject *parent) :
    QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> CameraFrameGrabber::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
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

bool CameraFrameGrabber::present(const QVideoFrame &frame)
{
    // source: http://code.qt.io/cgit/qt/qtmultimedia.git/commit/?id=a3879c70b30d80e33f135e8247f8e33846aaaba1

    if (frame.handleType() == QAbstractVideoBuffer::GLTextureHandle) {
        // Slow and inefficient path. Ideally what's on the GPU should remain on the GPU, instead of readbacks like this.
        QImage img(frame.width(), frame.height(), QImage::Format_RGBA8888);
        QOpenGLContext *ctx = new QOpenGLContext(this);
        ctx->create();
        //ctx->makeCurrent((QObject*) this);

        GLuint textureId = frame.handle().toUInt();
        QOpenGLFunctions *f = ctx->functions();
        GLuint fbo;
        f->glGenFramebuffers(1, &fbo);
        GLuint prevFbo;
        f->glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *) &prevFbo);
        f->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
        f->glReadPixels(0, 0, frame.width(), frame.height(), GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
        f->glBindFramebuffer(GL_FRAMEBUFFER, prevFbo);
        emit frameAvailable(img);
        return true;
    }
    else
    {
        if (!frame.isValid())
        {
            qWarning("imageFromVideoFrame: No mapped image data available for read");
            return false;
        }

        QImage::Format fmt = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
        if (fmt != QImage::Format_Invalid)
        {
            QVideoFrame cloneFrame(frame);
            cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
            QImage image = VideoFrameToImage(cloneFrame);
            emit frameAvailable(image);
            cloneFrame.unmap();
            return true;
        }

        qWarning("imageFromVideoFrame: No matching QImage format");
        return false;
    }

    return false;
}

QImage CameraFrameGrabber::VideoFrameToImage(QVideoFrame &cloneFrame)
{
    QImage image(cloneFrame.bits(),
                       cloneFrame.width(),
                       cloneFrame.height(),
                       QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));
    return(image);
}
