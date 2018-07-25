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
    void setInput(PipelineContainer<QImage> source);
    void handleResult();
private:
    QImage buffer;

    QFutureWatcher<QImage> watcher;

    int target_width;

    PipelineContainer<QImage> input_buffer;
    bool input_buffer_nonempty;

    /**
    * @brief Input id which is being processed
    */
    PipelineContainerInfo object_in_process;
public:
    ImageScaler(int target_width);

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QImage scale(QImage source);
    ImageScaler(const ImageScaler &that);
    ImageScaler();
};

#endif // IMAGESCALER_H
