/**
 * @file voidviewfinder.cpp
 * @brief A ViewFinder for QCamera which does nothing
 * when frames do arrive
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "voidviewfinder.h"
#include "qvideoframehelpers.h"

QList<QVideoFrame::PixelFormat> VoidViewFinder::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    Q_UNUSED(handleType);
    return QVideoFrameHelpers::supportedPixelFormats();
}
