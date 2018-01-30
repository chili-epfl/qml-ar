#include "fpscalculator.h"
#include <QDateTime>
#include <QtMath>
#include <QDebug>

void FPSCalculator::calculateMean()
{
    double mean = 0;
    QLinkedList<double>::iterator it;
    for(it = values.begin(); it != values.end(); it++)
    {
        mean += *it;
    }
    last_mean = mean / values.size();
}

void FPSCalculator::calculateStd()
{
    double std = 0;
    QLinkedList<double>::iterator it;
    for(it = values.begin(); it != values.end(); it++)
    {
        std += qPow(*it - last_mean, 2);
    }
    last_std = qSqrt(std / values.size());
}

FPSCalculator::FPSCalculator(int to_keep)
{
    values_to_keep = to_keep;
    values.clear();
    last_msecs = 0;
}

void FPSCalculator::newFrame()
{
    // obtaining msecs
    qint64 msecs = QDateTime::currentDateTime().currentMSecsSinceEpoch();

    // adding current freq
    if(last_msecs > 0 && msecs > last_msecs)
    {
        double freq = 1000. / (msecs - last_msecs);
        values.push_front(freq);
        if(values.size() > values_to_keep)
            values.removeLast();
    }

    // saving last timestamp
    last_msecs = msecs;

    // updating mean/std
    if(values.size() > 0)
    {
        calculateMean();
        calculateStd();
    }
}

double FPSCalculator::mean()
{
    return last_mean;
}

double FPSCalculator::std()
{
    return last_std;
}
