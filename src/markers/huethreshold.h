/**
 * @file huethreshold.h
 * @brief This class outputs binarized image
 * taking colors in [hue-delta, hue+delta]
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef HUETHRESHOLD_H
#define HUETHRESHOLD_H

#include "imageproviderasync.h"
#include "opencv2/highgui.hpp"
#include <QtConcurrent>

/** @brief Maximal number of pixels in an image
 */

#define IMAGE_MAX_PIXELS (640 * 480)

/**
 * @brief This class outputs binarized image
 * taking colors in [hue-delta, hue+delta]
 */

class HueThreshold : public ImageProviderAsync
{ Q_OBJECT
public:
    /** @brief Constructor */
    HueThreshold();

    virtual ~HueThreshold() {}

    /** @brief Copy constructor */
    HueThreshold(const HueThreshold &that);

private:
    /**
    * @brief Input id
    */
    PipelineContainerInfo object_in_process;

    /**
    * @brief Buffers
    */
    cv::Mat img;
    cv::Mat hsv;
    cv::Mat result;
    cv::Mat result_rgb;
    QImage result_qt;
    cv::Mat mask;

    uchar buf[IMAGE_MAX_PIXELS];

    /**
    * @brief For results from thread
    */
    QFutureWatcher<QImage> watcher;

    /**
     * @brief Mean hue (0-360)
     */
    int mean_h;

    /** @brief +- value for Mean Hue
     * @see mean_h
     */
    int delta_h;

    /**
    * @brief True if image is pending processing
    */
    int input_buffer_nonempty;

    /**
    * @brief Buffer for input images
    */
    PipelineContainer<QImage> input_buffer;

    /**
    * @brief Minimal and maximal SV values (0-255)
    */
    int min_s, max_s, min_v, max_v;

public slots:
    /**
    * @brief Set Hue to threshold on (Full scale: 0-360)
    */
    void setColor(double mean, double delta);

    /**
    * @brief Set input image
    */
    void setInput(PipelineContainer<QImage> input);

    /**
    * @brief Handle result from thread
    */
    void handleFinished();

    /**
     * @brief Set min/max Value (Full scale: 0-255)
     * @param min_ min value
     * @param max_ max value
     */
    void setVMinMax(double min_, double max_);

    /**
     * @brief Set min/max Saturation (Full scale: 0-255)
     * @param min_ min saturation
     * @param max_ max saturation
     */
    void setSMinMax(double min_, double max_);

    /**
    * @brief Loop implementation
    */
    QImage threshold(QImage source);
};

#endif // HUETHRESHOLD_H
