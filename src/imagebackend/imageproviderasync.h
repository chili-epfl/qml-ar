#ifndef IMAGEPROVIDERASYNC_H
#define IMAGEPROVIDERASYNC_H

#include <QQuickImageProvider>
#include <QObject>
#include <QImage>
#include "pipelinecontainer.h"

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
    void imageAvailable(PipelineContainer<QImage>);
};

#endif // IMAGEPROVIDERASYNC_H
