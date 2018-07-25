/**
 * @file cameraframegrabber.cpp
 * @brief This class implements a Viewfinder interface for the QCamera which stores images
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "cameraframegrabber.h"
#include "qvideoframehelpers.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QException>
#include <QOpenGLFramebufferObject>
#include <QDebug>
#include <QTextStream>
#include <QElapsedTimer>

CameraFrameGrabber::CameraFrameGrabber(QObject *parent) :
    QAbstractVideoSurface(parent)
{ Q_UNUSED(parent);
}

QList<QVideoFrame::PixelFormat> CameraFrameGrabber::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{ Q_UNUSED(handleType);
    return QVideoFrameHelpers::supportedPixelFormats();
}

bool CameraFrameGrabber::present(const QVideoFrame &frame)
{
    if (frame.handleType() == QAbstractVideoBuffer::GLTextureHandle)
        qFatal("Cannot work with OpenGL (are you using Android?)");

    if (!frame.isValid())
        qFatal("Camera frame is invalid");

    if (QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat())
            == QImage::Format_Invalid)
        qFatal("Camera frame format is invalid");

    QImage image = QVideoFrameHelpers::VideoFrameToImage(frame);
    emit frameAvailable(image);
    return true;
}
