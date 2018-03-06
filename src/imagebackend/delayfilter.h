#ifndef DELAYFILTER_H
#define DELAYFILTER_H

/*
 * This class delays VideoOutput by N frames
 * See https://forum.qt.io/topic/64498/buffer-delay-video-frames-from-qcamera/2
 */

#include <QVideoFilterRunnable>

class DelayFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable *createFilterRunnable();
signals:
    void finished(QObject *result);
};

#endif // DELAYFILTER_H
