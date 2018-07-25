/**
 * @file delayfilter.h
 * @brief This class delays VideoOutput by N frames
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef DELAYFILTER_H
#define DELAYFILTER_H

/**
 * @brief This class delays VideoOutput by N frames
 *
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
