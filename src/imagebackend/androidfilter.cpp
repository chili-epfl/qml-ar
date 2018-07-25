/**
 * @file androidfilter.cpp
 * @brief A class for fixing video output on android
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "androidfilter.h"

class AndroidFilterRunnable : public QVideoFilterRunnable
{
public:
    AndroidFilterRunnable()
    {
    }

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
