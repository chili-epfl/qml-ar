/**
 * @file huethreshold.cpp
 * @brief This class outputs binarized image
 * taking colors in [hue-delta, hue+delta]
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "huethreshold.h"
#include "timelogger.h"
#include "QtOpenCV/cvmatandqimage.h"
#include "opencv2/imgproc.hpp"
#include <strings.h>

HueThreshold::HueThreshold(const HueThreshold& that)
{
    this->min_v = that.min_v;
    this->max_v = that.max_v;
    this->min_s = that.min_s;
    this->max_s = that.max_s;
    this->mean_h = that.mean_h;
    this->delta_h = that.delta_h;
    input_buffer_nonempty = false;

}

HueThreshold::HueThreshold()
{
    min_s = min_v = 50;
    max_s = max_v = 255;
    input_buffer_nonempty = false;
    watcher.setParent(this);
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
}

QImage HueThreshold::threshold(QImage source)
{
    //Q_ASSERT(source.format() == QImage::Format_RGB888);
    TimeLoggerThroughput("%s", "[ANALYZE] Begin HueThresholdManual");

    QImage copied = source.copy();

    hsv = QtOcv::image2Mat_shared(copied);

    // rgb -> hsv
    cv::cvtColor(hsv, hsv, cv::COLOR_RGB2HSV, 3);

    int h_ = source.height();
    int w_ = source.width();

    // checking input size
    Q_ASSERT(h_ * w_ < IMAGE_MAX_PIXELS);

    uchar* src = hsv.data;

    uchar mean_h_ = (int) (mean_h / 2);
    uchar min_s_ = min_s;
    uchar max_s_ = max_s;
    uchar min_v_ = min_v;
    uchar max_v_ = max_v;
    uchar delta_h_ = delta_h / 2;

    for(int j = 0; j < h_; j++)
        for(int i = 0; i < w_; i++)
        {
            uchar* pixel = src + 3 * (w_ * j + i);
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
            else buf[w_ * j + i] = 0;
        }

    QImage result(buf, w_, h_, QImage::Format_Grayscale8);

    QImage result_copy = result.copy();

    TimeLoggerThroughput("%s", "[ANALYZE] End HueThresholdManual");

    return result_copy;
}

void HueThreshold::setColor(double mean, double delta)
{
    TimeLoggerLog("Got color %.2f +- %.2f", mean, delta);

    mean_h = mean;
    delta_h = delta;
}

void HueThreshold::setVMinMax(double min_, double max_)
{
    min_v = min_;
    max_v = max_;
}

void HueThreshold::setSMinMax(double min_, double max_)
{
    min_s = min_;
    max_s = max_;
}

void HueThreshold::setInput(PipelineContainer<QImage> input)
{
    input_buffer = input;

    if(!watcher.isRunning())
    {
        object_in_process = input.info();
        QFuture<QImage> future = QtConcurrent::run(*this, &HueThreshold::threshold, input.o());
        watcher.setFuture(future);
    }
    else input_buffer_nonempty = true;
}

void HueThreshold::handleFinished()
{
    QImage result = watcher.result();

    emit imageAvailable(PipelineContainer<QImage>
                        (result, object_in_process.checkpointed("HueThreshold")));

    if(input_buffer_nonempty)
    {
        input_buffer_nonempty = false;
        setInput(input_buffer);
    }
}
