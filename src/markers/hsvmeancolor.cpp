/**
 * @file hsvmeancolor.cpp
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "hsvmeancolor.h"
#include <QtMath>

HSVMeanColor::HSVMeanColor()
{
    h_vector.setX(0);
    h_vector.setY(0);
}

void HSVMeanColor::addColor(int h)
{
    /**
    * @brief Hue in radians
    */
    double h_radians = qDegreesToRadians((double) h);

    /**
    * @brief Adding this to sum
    */
    h_vector += QVector2D(qCos(h_radians), qSin(h_radians));

    /**
    * @brief Saving h
    */
    h_values.append(h);
}

void HSVMeanColor::addColor(QColor c)
{
    /**
    * @brief Obtaining h, s, v
    */
    int h, s, v;
    c.getHsv(&h, &s, &v);
    addColor(h);
}

double HSVMeanColor::meanHue()
{
    /**
    * @brief Returning angle of the vector
    */
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
    /**
    * @brief Mean color
    */
    double mean = meanHue();

    /**
    * @brief Resulting sigma
    */
    double result = 0;

    /**
    * @brief Adding (xi - mu) ^2
    */
    QVector<double>::iterator it;
    for(it = h_values.begin(); it != h_values.end(); it++)
        result += qPow(angleDifference(mean, *it), 2);

    /**
    * @brief Dividing by N
    */
    result /= h_values.size();

    /**
    * @brief Taking square
    */
    result = qSqrt(result);

    /**
    * @brief Resulting sigma
    */
    return result;
}
