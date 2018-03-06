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
