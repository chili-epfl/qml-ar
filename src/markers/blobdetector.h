#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H

#include <QImage>
#include "opencv2/features2d.hpp"

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

    // cv blob detector
    cv::Ptr<cv::SimpleBlobDetector> detector;
public:
    BlobDetector();
    virtual ~BlobDetector() {}

    // detect blobs on a qimage
    QImage detectBlobs(QImage source, int max_blobs = -1);
};

#endif // BLOBDETECTOR_H
