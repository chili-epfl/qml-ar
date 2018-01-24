#include "hsvintervaldetector.h"
#include "qvideoframehelpers.h"
#include "timelogger.h"
#include <QRgb>
#include <QColor>

HSVIntervalDetector::HSVIntervalDetector()
{
}

void HSVIntervalDetector::newPoints(QPair<QImage, QVector<QVector2D>> image_points)
{
    // unpacking arguments
    QImage image = image_points.first;
    QVector<QVector2D> points = image_points.second;

    // doing nothing on empty image
    if(image.width() * image.height() <= 1)
        return;

    // detecting min and max HSV range
    QVector<QVector2D>::iterator it;
    for(it = points.begin(); it != points.end(); it++)
    {
        // image point
        QVector2D point = *it;

        // pixel color
        QColor pixel = image.pixel(point.x(), point.y());
        colors.append(pixel);
    }
}

void HSVIntervalDetector::printAll()
{
    // resulting HSV values
    int h, s, v;

    // loop over all found colors
    QVector<QColor>::iterator it;
    for(it = colors.begin(); it != colors.end(); it++)
    {
        // color of the pixel
        QColor pixel = *it;

        // getting HSV colors
        pixel.getHsv(&h, &s, &v);
        TimeLoggerLog("HSV %d %d %d", h, s, v);
    }
}
