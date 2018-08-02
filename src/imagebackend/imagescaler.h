/**
 * @file imagescaler.h
 * @brief This class scales images
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <QImage>
#include <QtConcurrent>
#include "imageproviderasync.h"

/**
 * @brief This class scales images
 */

class ImageScaler : public ImageProviderAsync
{ Q_OBJECT
public slots:
    /**
     * @brief Set new input to the scaler
     * @param source the unscaled QImage
     */
    void setInput(PipelineContainer<QImage> source);

    /**
     * @brief Called when separate thread finishes
     */
    void handleResult();
private:
    /**
     * @brief buffer for scaling
     */
    QImage buffer;

    /**
     * @brief watcher for the async processing
     */
    QFutureWatcher<QImage> watcher;

    /**
     * @brief Requested image width
     */
    int target_width;

    /**
     * @brief Buffer with input image
     */
    PipelineContainer<QImage> input_buffer;

    /**
     * @brief True if there is an image in input buffer waiting processing
     */
    bool input_buffer_nonempty;

    /**
    * @brief Input id which is being processed
    */
    PipelineContainerInfo object_in_process;
public:
    /**
     * @brief ImageScaler constructor
     * @param target_width The requested image width
     */
    ImageScaler(int target_width);

    /**
     * @brief requestImage for QML
     */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    /**
     * @brief scale image
     * @param source input image
     * @return scaled image
     */
    QImage scale(QImage source);

    /**
     * @brief ImageScaler copy contructor
     */
    ImageScaler(const ImageScaler &that);

    /**
     * @brief ImageScaler constructor
     */
    ImageScaler();
};

#endif // IMAGESCALER_H
