#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include <QImage>
#include "opencv2/features2d.hpp"
#include <vector>

/*
 * This class detects blobs on a QImage
 * and returns a QImage with white pixels
 * at blob positions
 */

class BlobDetector : public QObject
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
public:
    BlobDetector();
    virtual ~BlobDetector() {}

    // detect blobs on a qimage
    void detectBlobs(QImage source, int max_blobs);

    // draws resulting blobs
    QImage drawBlobs();

    // return the blobs
    std::vector<cv::KeyPoint> getBlobs();
};

#endif // BLOBDETECTOR_H
