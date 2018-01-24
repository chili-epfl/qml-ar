#include "huethreshold.h"
#include "timelogger.h"
#include "QtOpenCV/cvmatandqimage.h"
#include "opencv2/imgproc.hpp"

HueThreshold::HueThreshold(const HueThreshold& that) : HueThreshold()
{
    this->min_s = that.min_s;
    this->max_s = that.max_s;
    this->min_hsv = that.min_hsv;
    this->max_hsv = that.max_hsv;
}

HueThreshold::HueThreshold()
{
    min_s = min_v = 30;
    max_s = max_v = 255;
    input_buffer_nonempty = false;
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
}

QImage HueThreshold::threshold(QImage source)
{
    TimeLoggerLog("%s", "Threshold begin");

    // nothing on no ranges
    if(min_hsv.length() == 0)
        return source;

    TimeLoggerLog("%s", "Converting to HSV");
    // qt -> cv input
    cv::Mat img = QtOcv::image2Mat_shared(source);

    // rgb -> hsv
    static cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // resulting mask
    static cv::Mat result;

    // sanity check
    Q_ASSERT(min_hsv.length() == max_hsv.length());

    TimeLoggerLog("%s", "Thresholding");

    for(int i = 0; i < min_hsv.length(); i++)
    {
        // current mask
        cv::Mat mask;
        cv::inRange(hsv, min_hsv.at(i), max_hsv.at(i), mask);

        // result = mask or result|mask
        if(i == 0) result = mask;
        else cv::bitwise_or(mask, result, result);
    }

    // converting to rgb
    static cv::Mat result_rgb;
    cv::cvtColor(255 - result, result_rgb, cv::COLOR_GRAY2RGB);

    // resulting mask in qt
    QImage result_qt = QtOcv::mat2Image(result_rgb);
    result_qt.toPixelFormat(QImage::Format_RGB888);

    TimeLoggerLog("%s", "Done");

    return result_qt;
}

void HueThreshold::addMinHue(double hue)
{
    min_hsv.append(cv::Scalar(hue / 2, min_s, min_v));
}

void HueThreshold::addMaxHue(double hue)
{
    max_hsv.append(cv::Scalar(hue / 2, max_s, max_v));
}

void HueThreshold::setColor(double mean, double sigma)
{
    TimeLoggerLog("Got color %.2f +- %.2f", mean, sigma);

    // using 3 sigma
    double delta = 3 * sigma;

    // saving color range as thresholds

    if(mean - delta < 0)
    {
        // [0, m]
        addMinHue(0);
        addMaxHue(mean);

        // [m-s+360, 360]
        addMinHue(mean - delta + 359);
        addMaxHue(359);
    }
    else
    {
        // [m-s, m]
        addMinHue(mean - delta);
        addMaxHue(mean);
    }

    if(mean + delta > 360)
    {
        // [0, m+s-360]
        addMinHue(0);
        addMaxHue(mean + delta - 359);

        // [m, 360]
        addMinHue(mean);
        addMaxHue(359);
    }
    else
    {
        // [m, m+s]
        addMinHue(mean);
        addMaxHue(mean + delta);
    }

    // sanity check
    Q_ASSERT(min_hsv.length() == max_hsv.length());

    for(int i = 0; i < min_hsv.length(); i++)
    {
        TimeLoggerLog("Thresholding from (%.2f %.2f %.2f) to (%.2f %.2f %.2f)",
                      min_hsv[i][0], min_hsv[i][1], min_hsv[i][2],
                      max_hsv[i][0], max_hsv[i][1], max_hsv[i][2]);
    }
}

void HueThreshold::setInput(QImage input)
{
    input_buffer = input;

    if(!watcher.isRunning())
    {
        QFuture<QImage> future = QtConcurrent::run(*this, &HueThreshold::threshold, input);
        watcher.setFuture(future);
    }
    else input_buffer_nonempty = true;
}

void HueThreshold::handleFinished()
{
    QImage result = watcher.result();

    TimeLoggerLog("Image from threshold %dx%d", result.width(), result.height());

    emit imageAvailable(result);

    if(input_buffer_nonempty)
    {
        input_buffer_nonempty = false;
        setInput(input_buffer);
    }
}
