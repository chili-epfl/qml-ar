#ifndef IMAGEPROVIDERASYNC_H
#define IMAGEPROVIDERASYNC_H

#include <QQuickImageProvider>
#include <QObject>
#include <QImage>

class ImageProviderAsync : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    ImageProviderAsync();

    virtual ~ImageProviderAsync() {}

signals:
    void imageAvailable(QImage);
};

#endif // IMAGEPROVIDERASYNC_H
