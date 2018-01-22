#ifndef ANDROIDFILTER_H
#define ANDROIDFILTER_H

#include <QAbstractVideoFilter>

/*
 * A class for fixing video output on android
 * see https://bugreports.qt.io/browse/QTBUG-48567
 * response by Christoph Keller, 29 Apr '17 12:57 PM
 */

class AndroidFilter : public QAbstractVideoFilter
{
    Q_OBJECT

public:
    explicit AndroidFilter(QObject *parent = 0);

    // QAbstractVideoFilter interface
    QVideoFilterRunnable *createFilterRunnable() override;
};

#endif // ANDROIDFILTER_H
