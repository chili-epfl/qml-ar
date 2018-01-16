#include "blobdetector.h"
#include "opencv2/imgproc.hpp"
#include "QtOpenCV/cvmatandqimage.h"
#include "opencv2/features2d.hpp"
#include "config.h"
#include <vector>

BlobDetector::BlobDetector()
{
    // set blob detector parameters
    cv::SimpleBlobDetector::Params parameters;
    parameters.minThreshold = 0;
    parameters.maxThreshold = 255;
    parameters.filterByArea = true;
    parameters.minArea = 1;
    parameters.filterByCircularity = true;
    parameters.minCircularity = 0.8;
    parameters.filterByConvexity = false;
    parameters.minConvexity = 0.87;
    parameters.filterByInertia = false;
    parameters.minInertiaRatio = 0.01;

    // create blob detector
    detector = cv::SimpleBlobDetector::create(parameters);

    // init blur size parameter
    blur_size.height = 2;
    blur_size.width = 2;
}

QImage BlobDetector::detectBlobs(QImage source, int max_blobs)
{
    // get data qimage -> mat
    cv::Mat source_cv = QtOcv::image2Mat_shared(source);

    cv::Mat source_cv_gray;
    cv::cvtColor(source_cv, source_cv_gray, cv::COLOR_RGB2GRAY);

    // blur the image
    cv::Mat blurred;
    cv::blur(source_cv_gray, blurred, blur_size);

    // detect blobs
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(blurred, keypoints);

    // store the result in mat
    cv::Mat result(source_cv_gray.rows, source_cv_gray.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    // go through all blobs
    std::vector<cv::KeyPoint>::iterator it;
    int i = 0;
    for(it = keypoints.begin(); it != keypoints.end(); it++, i++)
    {
        if(max_blobs > 0 && i >= max_blobs) break;
        // draw a white circle on the place of a blob
        cv::KeyPoint kp = (*it);
        cv::circle(result, kp.pt, 1, cv::Scalar(255, 255, 255), -1);
    }

    // return mat -> qt image
    return QtOcv::mat2Image(result);
}
