/**
 * @file hsvintervaldetector.cpp
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "hsvintervaldetector.h"
#include "qvideoframehelpers.h"
#include "timelogger.h"
#include <QtMath>
#include <QRgb>
#include <QColor>

HSVIntervalDetector::HSVIntervalDetector(int min_points)
{
    this->min_points = min_points;
}

void HSVIntervalDetector::newPoints(PipelineContainer<QPair<QImage, QVector<QVector2D>>> image_points)
{
    /**
    * @brief Unpacking arguments
    */
    QImage image = image_points.o().first;
    QVector<QVector2D> points = image_points.o().second;

    /**
    * @brief Doing nothing on empty image
    */
    if(image.width() * image.height() <= 1)
        return;

    /**
    * @brief Updating hsv color vector
    */
    QVector<QVector2D>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
    {
    /**
    * @brief Image point
    */
        QVector2D point = *it;

    /**
    * @brief Pixel color
    */
        QColor pixel = image.pixel(point.x(), point.y());
        colors.append(pixel);

    /**
    * @brief Get HSV
    */
        int h, s, v;
        pixel.getHsv(&h, &s, &v);

        mean_hue.addColor(h);
        mean_s.addValue(s);
        mean_v.addValue(v);
    }

    /**
    * @brief Calculating results after minimal number of points
    */
    if(colors.length() > min_points)
        calculate();
}

void HSVIntervalDetector::calculate()
{
    /**
    * @brief Doing nothing on no colors
    */
    if(colors.size() == 0) return;

    /**
    * @brief Returning mean color and standard deviation
    */
    emit hAvailable(mean_hue.meanHue(), mean_hue.stdHue());

    /**
    * @brief Get SV
    */
    emit sAvailable(mean_s.getMean(), mean_s.getStd());
    emit vAvailable(mean_v.getMean(), mean_v.getStd());
}
