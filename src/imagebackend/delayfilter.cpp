#include <QLinkedList>
#include <QVideoFrame>
#include "delayfilter.h"

class DelayFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

QVideoFrame DelayFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{ Q_UNUSED(surfaceFormat) Q_UNUSED(flags)

    static QLinkedList<QVideoFrame> buffer;
    buffer.push_back(QVideoFrame(*input));

    QVideoFrame result = buffer.last();

    //if (buffer.size() >= 10)
    //    buffer.pop_front();

    return result;
}

QVideoFilterRunnable *DelayFilter::createFilterRunnable()
{
    return new DelayFilterRunnable;
}
