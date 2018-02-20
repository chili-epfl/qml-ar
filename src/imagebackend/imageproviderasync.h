#ifndef IMAGEPROVIDERASYNC_H
#define IMAGEPROVIDERASYNC_H

#include <QQuickImageProvider>
#include <QObject>
#include <QImage>

/*
 * This class is an interface for image providers which emit a signal
 * on new image
 */

class ImageProviderAsync : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    ImageProviderAsync();

    virtual ~ImageProviderAsync() {}

signals:
    void imageAvailable(QImage);
};

#endif // IMAGEPROVIDERASYNC_H
