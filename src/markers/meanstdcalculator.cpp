/**
 * @file meanstdcalculator.cpp
 * @brief This class calculates mean and std values
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "meanstdcalculator.h"
#include <QtMath>

MeanStdCalculator::MeanStdCalculator()
{

}

void MeanStdCalculator::addValue(double value)
{
    values.append(value);
}

double MeanStdCalculator::getMean()
{
    if(values.size() == 0) return 0;

    double result = 0;
    QVector<double>::iterator it;
    for(it = values.begin(); it != values.end(); it++)
        result += *it;
    return(result / values.size());
}

double MeanStdCalculator::getStd()
{
    if(values.size() == 0) return 0;
    double mean = getMean();
    double result = 0;

    QVector<double>::iterator it;
    for(it = values.begin(); it != values.end(); it++)
        result += qPow(*it - mean, 2);

    return qSqrt(result / values.size());
}
