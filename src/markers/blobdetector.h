#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include <QtConcurrent>
#include <QImage>
#include "opencv2/features2d.hpp"
#include "imageproviderasync.h"
#include <vector>

/*
 * This class detects blobs on a QImage
 * and returns a QImage with white pixels
 * at blob positions
 */

class BlobDetector : public ImageProviderAsync
{ Q_OBJECT
private:
    // size for blurring the image
    cv::Size blur_size;

    // cv blob detector with parameters
    cv::SimpleBlobDetector::Params parameters;
    cv::Ptr<cv::SimpleBlobDetector> detector;

    // detected blobs
    std::vector<cv::KeyPoint> keypoints;

    // last input image
    QImage last_input;

    // true if detectBlobs was called
    bool is_initialized;

    // resulting matrix
    cv::Mat result;

    // buffered output
    QImage last_output;

    // for handling background job
    QFutureWatcher<QImage> watcher;

    // maximum numbers of dots to detect
    int max_blobs;

    // input buffer for background task
    bool buffer_is_nonempty;
    QImage input_buffer;
public:
    BlobDetector();
    virtual ~BlobDetector() {}
    BlobDetector(const BlobDetector &detector);

    // detect blobs on a qimage
    void detectBlobs(QImage source);

    // draws resulting blobs
    QImage drawBlobs();

    // return the blobs
    std::vector<cv::KeyPoint> getBlobs();

    // returns drawn blobs
    QImage getAndDraw(QImage img);

    // returns last image
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
public slots:
    // set input to background thread
    void setInput(QImage img);

    // get result from background thread
    void handleFinished();
};

#endif // BLOBDETECTOR_H
