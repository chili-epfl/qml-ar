/**
 * @file nv21videofilter.h
 * @brief This class manages NV21VideoFilterRunnable. Only for Android API 26 and higher.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef NV21VIDEOFILTER_H
#define NV21VIDEOFILTER_H

#include <QAbstractVideoFilter>
#include "imageproviderasync.h"

class NV21VideoFilterRunnable;

/**
 * @brief The NV21VideoFilter class manages NV21VideoFilterRunnable.  Only for Android API 26 and higher.
 *
 * Based on https://github.com/aseba-community/thymio-ar/blob/eb942a5e96761303512a07dc6e5057749ff8939e/vision-video-filter.h
 */

class NV21VideoFilter : public QAbstractVideoFilter
{ Q_OBJECT
public:
    /**
     * @brief NV21VideoFilter constructor
     * @param parent Parent Qt object
     */
    explicit NV21VideoFilter(QObject* parent = 0);

    /**
     * @brief createFilterRunnable creates new video filter
     * @return NV21VideoFilterRunnable object
     */
    QVideoFilterRunnable* createFilterRunnable();

    /**
     * @brief runnable holds the last Runnable object returned by createFilterRunnable
     */
    static NV21VideoFilterRunnable* runnable;
};

#endif // NV21VIDEOFILTER_H
