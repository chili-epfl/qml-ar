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
    timestamps.clear();
    last_mean = 0;
    last_std = 0;
    frames_window = 20;
}

void FPSCalculator::newFrame()
{
    // obtaining msecs
    qint64 msecs = QDateTime::currentMSecsSinceEpoch();

    // adding current timestamp
    timestamps.push_back(msecs);

    // calculating fps based on window
    if(timestamps.size() >= frames_window)
    {
        // removing extra timestamps
        if(timestamps.size() > frames_window)
            timestamps.removeFirst();

        // FPS = 1000. / (DT / N)
        double freq = 1000. / (timestamps.last() - timestamps.first()) * frames_window;

        // saving frequency
        values.push_front(freq);
        if(values.size() > values_to_keep)
            values.removeLast();
    }

    // updating mean/std
    if(values.size() > 0)
    {
        calculateMean();
        emit newMean(last_mean);
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
