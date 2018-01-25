#include "huethreshold.h"
#include "timelogger.h"
#include "QtOpenCV/cvmatandqimage.h"
#include "opencv2/imgproc.hpp"
#include <strings.h>

HueThreshold::HueThreshold(const HueThreshold& that) : HueThreshold()
{
    this->min_v = that.min_v;
    this->max_v = that.max_v;
    this->min_s = that.min_s;
    this->max_s = that.max_s;
    this->min_hsv = that.min_hsv;
    this->max_hsv = that.max_hsv;
    this->mean_h = that.mean_h;
    this->delta_h = that.delta_h;
}

HueThreshold::HueThreshold()
{
    min_s = min_v = 50;
    max_s = max_v = 255;
    input_buffer_nonempty = false;
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
}

QImage HueThreshold::thresholdManual(QImage source)
{
    //Q_ASSERT(source.format() == QImage::Format_RGB888);
    TimeLoggerLog("%s", "[ANALYZE] Begin HueThresholdManual");

    hsv = QtOcv::image2Mat_shared(source);

    // rgb -> hsv
    cv::cvtColor(hsv, hsv, cv::COLOR_RGB2HSV);

    int h_ = source.height();
    int w_ = source.width();

    uchar* src = hsv.data;

    bzero(buf, 640*480);

    uchar mean_h_ = (int) (mean_h / 2);
    uchar min_s_ = min_s;
    uchar max_s_ = max_s;
    uchar min_v_ = min_v;
    uchar max_v_ = max_v;
    uchar delta_h_ = delta_h / 2;

    for(int j = 0; j < h_; j++)
    for(int i = 0; i < w_; i++)
        {
            uchar* pixel = src + (w_ * j + i);
            uchar h = pixel[0];
            uchar s = pixel[1];
            uchar v = pixel[2];

            uchar diff;
            if(h > mean_h_) diff = h - mean_h_;
            else diff = mean_h_ - h;
            if(180 - diff < diff) diff = 180 - diff;

            if(s >= min_s_ && s <= max_s_ &&
                    v >= min_v_ && v <= max_v_ &&
                    diff < delta_h_)
            {
                buf[w_ * j + i] = 255;
            }
        }

    QImage result(buf, w_, h_, QImage::Format_Grayscale8);

    TimeLoggerLog("%s", "[ANALYZE] End HueThresholdManual");
    return result;
}

QImage HueThreshold::threshold(QImage source)
{
    TimeLoggerLog("%s", "[ANALYZE] Begin HueThreshold");

    // nothing on no ranges
    if(min_hsv.length() == 0)
        return source;

    TimeLoggerLog("%s", "Converting to HSV");
    // qt -> cv input
    img = QtOcv::image2Mat(source);

    // rgb -> hsv
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    // sanity check
    Q_ASSERT(min_hsv.length() == max_hsv.length());

    TimeLoggerLog("%s", "Thresholding");

    for(int i = 0; i < min_hsv.length(); i++)
    {
        // current mask
        cv::inRange(hsv, min_hsv.at(i), max_hsv.at(i), mask);

        // result = mask or result|mask
        if(i == 0) result = mask;
        else cv::bitwise_or(mask, result, result);

        TimeLoggerLog("%s", "End step");
    }

    TimeLoggerLog("%s", "End mask");

    // converting to rgb
    cv::cvtColor(255 - result, result_rgb, cv::COLOR_GRAY2RGB);

    TimeLoggerLog("%s", "End GRAY->RGB");

    // resulting mask in qt
    result_qt = QtOcv::mat2Image(result_rgb);

    TimeLoggerLog("%s", "End mat2Image");

    result_qt.toPixelFormat(QImage::Format_RGB888);

    TimeLoggerLog("%s", "[ANALYZE] End HueThreshold");

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

    // using sigma
    double delta = sigma;

    mean_h = mean;
    delta_h = sigma;

    // saving color range as thresholds
    if(mean - delta < 0)
    {
        // [0, m]
        addMinHue(0);
        addMaxHue(mean);

        // [m-s+360, 360]
        addMinHue(mean - delta + 360);
        addMaxHue(360);
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
        addMaxHue(mean + delta - 360);

        // [m, 360]
        addMinHue(mean);
        addMaxHue(360);
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

void HueThreshold::setV(double mean, double std)
{
    min_v = mean - std;
    max_v = mean + std;
}

void HueThreshold::setS(double mean, double std)
{
    min_s = mean - std;
    max_s = mean + std;
}

void HueThreshold::setInput(QImage input)
{
    input_buffer = input;

    if(!watcher.isRunning())
    {
        //QFuture<QImage> future = QtConcurrent::run(*this, &HueThreshold::threshold, input);
        QFuture<QImage> future = QtConcurrent::run(*this, &HueThreshold::thresholdManual, input);
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
