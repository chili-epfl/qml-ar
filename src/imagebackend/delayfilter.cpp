/**
 * @file delayfilter.cpp
 * @brief This class delays VideoOutput by N frames
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include <QLinkedList>
#include <QVideoFrame>
#include <QDebug>
#include "delayfilter.h"
#include "qvideoframehelpers.h"

class DelayFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

QVideoFrame DelayFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{ Q_UNUSED(surfaceFormat) Q_UNUSED(flags)

    static QLinkedList<QVideoFrame> buffer;
    buffer.push_back(*input);
    qDebug() << "Add texture" << input->handle().toUInt() << input->size();

    QVideoFrame result = buffer.first();

    if (buffer.size() >= 5)
        buffer.pop_front();

    qDebug() << "buffer size: " << buffer.size();

    return result;
}

QVideoFilterRunnable *DelayFilter::createFilterRunnable()
{
    return new DelayFilterRunnable;
}
