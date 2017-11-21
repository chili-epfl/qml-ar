#include "voidviewfinder.h"
#include <qvideoframehelpers.h>

QList<QVideoFrame::PixelFormat> VoidViewFinder::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QVideoFrameHelpers::supportedPixelFormats();
}
