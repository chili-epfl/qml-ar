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

    QImage last_output;

    QFutureWatcher<QImage> watcher;

    int max_blobs;
public:
    BlobDetector();
    virtual ~BlobDetector() {}

    // detect blobs on a qimage
    void detectBlobs(QImage source);

    // draws resulting blobs
    QImage drawBlobs();

    // return the blobs
    std::vector<cv::KeyPoint> getBlobs();
    BlobDetector(BlobDetector &detector);
    void handleFinished();

public slots:
    void setInput(QImage img);
};

#endif // BLOBDETECTOR_H
