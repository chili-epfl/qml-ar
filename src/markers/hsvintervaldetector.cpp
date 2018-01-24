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

void HSVIntervalDetector::newPoints(QPair<QImage, QVector<QVector2D>> image_points)
{
    // unpacking arguments
    QImage image = image_points.first;
    QVector<QVector2D> points = image_points.second;

    // doing nothing on empty image
    if(image.width() * image.height() <= 1)
        return;

    // updating hsv color vector
    QVector<QVector2D>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
    {
        // image point
        QVector2D point = *it;

        // pixel color
        QColor pixel = image.pixel(point.x(), point.y());
        colors.append(pixel);

        // get HSV
        int h, s, v;
        pixel.getHsv(&h, &s, &v);

        mean_hue.addColor(h);
        mean_s.addValue(s);
        mean_v.addValue(v);
    }

    // calculating results after minimal number of points
    if(colors.length() > min_points)
        calculate();
}

void HSVIntervalDetector::calculate()
{
    // doing nothing on no colors
    if(colors.size() == 0) return;

    // returning mean color and standard deviation
    emit hAvailable(mean_hue.meanHue(), mean_hue.stdHue());

    // get SV
    emit sAvailable(mean_s.getMean(), mean_s.getStd());
    emit vAvailable(mean_v.getMean(), mean_v.getStd());
}
