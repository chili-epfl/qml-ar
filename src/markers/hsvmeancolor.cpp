#include "hsvmeancolor.h"
#include <QtMath>

HSVMeanColor::HSVMeanColor()
{
    h_vector.setX(0);
    h_vector.setY(0);
}

void HSVMeanColor::addColor(int h)
{
    // hue in radians
    double h_radians = qDegreesToRadians((double) h);

    // adding this to sum
    h_vector += QVector2D(qCos(h_radians), qSin(h_radians));

    // saving h
    h_values.append(h);
}

void HSVMeanColor::addColor(QColor c)
{
    // obtaining h, s, v
    int h, s, v;
    c.getHsv(&h, &s, &v);
    addColor(h);
}

double HSVMeanColor::meanHue()
{
    // returning angle of the vector
    return qRadiansToDegrees(qAtan2(h_vector.y(), h_vector.x()));
}

double HSVMeanColor::angleDifference(double a1, double a2)
{
    double result = qAbs(a1 - a2);
    if(360 - result < result) result = 360 - result;
    return result;
}

double HSVMeanColor::stdHue()
{
    // mean color
    double mean = meanHue();

    // resulting sigma
    double result = 0;

    // adding (xi - mu) ^2
    QVector<double>::iterator it;
    for(it = h_values.begin(); it != h_values.end(); it++)
        result += qPow(angleDifference(mean, *it), 2);

    // dividing by N
    result /= h_values.size();

    // taking square
    result = qSqrt(result);

    // resulting sigma
    return result;
}
