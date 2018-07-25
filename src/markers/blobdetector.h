/**
 * @file blobdetector.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include <QtConcurrent>
#include <QImage>
#include "opencv2/features2d.hpp"
#include "imageproviderasync.h"
#include <vector>
#include <QVector>
#include <QVector2D>

/*
 * This class detects blobs on a QImage
 * and returns a QImage with white pixels
 * at blob positions
 */

class BlobDetector : public ImageProviderAsync
{ Q_OBJECT
private:
    /**
    * @brief Size for blurring the image
    */
    cv::Size blur_size;

    /**
    * @brief Cv blob detector with parameters
    */
    cv::SimpleBlobDetector::Params parameters;
    cv::Ptr<cv::SimpleBlobDetector> detector;

    /**
    * @brief Detected blobs
    */
    std::vector<cv::KeyPoint> keypoints;

    /**
    * @brief Last input image
    */
    QImage last_input;

    /**
    * @brief True if detectBlobs was called
    */
    bool is_initialized;

    /**
    * @brief Resulting matrix
    */
    cv::Mat result;

    /**
    * @brief Buffered output
    */
    QImage last_output;

    /**
    * @brief For handling background job
    */
    QFutureWatcher<QPair<QVector<QVector2D>, QImage>> watcher;

    /**
    * @brief Maximum numbers of dots to detect
    */
    int max_blobs;

    /**
    * @brief Input buffer for background task
    */
    bool buffer_is_nonempty;
    QImage input_buffer;
public:
    BlobDetector();
    virtual ~BlobDetector() {}
    BlobDetector(const BlobDetector &detector);

    BlobDetector(int max_blobs);

    /**
    * @brief Detect blobs on a qimage
    */
    QVector<QVector2D> detectBlobs(QImage source);

    /**
    * @brief Draws resulting blobs
    */
    QImage drawBlobs();

    /**
    * @brief Return the blobs
    */
    std::vector<cv::KeyPoint> getBlobs();

    /**
    * @brief Returns drawn blobs
    */
    QPair<QVector<QVector2D>, QImage> getAndDraw(QImage img);

    /**
    * @brief Returns last image
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
public slots:
    /**
    * @brief Set input to background thread
    */
    void setInput(PipelineContainer<QImage> img);

    /**
    * @brief Get result from background thread
    */
    void handleFinished();
signals:
    /**
    * @brief Emitted on new blobs
    * Need Qt::QueuedConnection
    */
    void blobsUpdated(QVector<QVector2D>);
};

#endif // BLOBDETECTOR_H
