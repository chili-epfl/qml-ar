/**
 * @file androidfilter.cpp
 * @brief A class for fixing video output on android
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "androidfilter.h"

/** @brief The filter which maps and unmaps data
 * see https://bugreports.qt.io/browse/QTBUG-48567
 * response by Christoph Keller, 29 Apr '17 12:57 PM
 */
class AndroidFilterRunnable : public QVideoFilterRunnable
{
public:
    /** @brief Constructor */
    AndroidFilterRunnable() {}

    /** @brief Map and unmap */
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags) override;
};

QVideoFrame AndroidFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{ Q_UNUSED(surfaceFormat) Q_UNUSED(flags)
    return QVideoFrameHelpers::cloneAndroidFrame(input);
}

AndroidFilter::AndroidFilter(QObject *parent)
: QAbstractVideoFilter(parent)
{
}

QVideoFilterRunnable *AndroidFilter::createFilterRunnable()
{
    return new AndroidFilterRunnable;
}
