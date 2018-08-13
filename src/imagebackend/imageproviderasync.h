/**
 * @file imageproviderasync.h
 * @brief This class is an interface for image providers which emit a signal
 * on new image
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef IMAGEPROVIDERASYNC_H
#define IMAGEPROVIDERASYNC_H

#include <QQuickImageProvider>
#include <QObject>
#include <QImage>
#include "pipelinecontainer.h"

/**
 * @brief This class is an interface for image providers which emit a signal
 * on new image
 */

class ImageProviderAsync : public QObject, public QQuickImageProvider
{ Q_OBJECT
public:
    /**
     * @brief ImageProviderAsync constructor
     */
    ImageProviderAsync();

    virtual ~ImageProviderAsync() {}

signals:
    /**
     * @brief imageAvailable emitted when new image is ready
     * @param image the ready image
     */
    void imageAvailable(PipelineContainer<QImage> image);

protected:
    /**
     * @brief Describes if the object is currently active
     */
    bool is_active = true;

public slots:
    /**
     * @brief Make this object active/inactive
     * @param active The new active/inactive parameter
     */
    void setActive(bool active) {is_active = active;}
};

#endif // IMAGEPROVIDERASYNC_H
