/**
 * @file androidfilter.h
 * @brief A class for fixing video output on android
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef ANDROIDFILTER_H
#define ANDROIDFILTER_H

#include <QAbstractVideoFilter>
#include "qvideoframehelpers.h"

/**
 * @brief A class for fixing video output on android
 *
 * see https://bugreports.qt.io/browse/QTBUG-48567
 * response by Christoph Keller, 29 Apr '17 12:57 PM
 */

class AndroidFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    explicit AndroidFilter(QObject *parent = 0);

    /**
    * @brief QAbstractVideoFilter interface
    */
    QVideoFilterRunnable *createFilterRunnable() override;
};

#endif // ANDROIDFILTER_H
