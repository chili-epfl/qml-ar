#include "latencycalculator.h"
#include <QtMath>

void LatencyCalculator::update()
{
    // calculating mean
    double new_mean = 0;
    QLinkedList<qint64>::iterator it;
    for(it = latency_ms.begin(); it != latency_ms.end(); it++)
        new_mean += *it;
    new_mean /= latency_ms.size();

    // calculating std
    double new_std = 0;
    for(it = latency_ms.begin(); it != latency_ms.end(); it++)
        new_std += qPow(*it - new_mean, 2);
    new_std /= latency_ms.size();
    new_std = qSqrt(new_std);

    // saving
    latency_mean = new_mean;
    latency_std = new_std;

    // telling others
    emit newMean(latency_mean);
}

LatencyCalculator::LatencyCalculator()
{
    // using 100 iterations
    frames_window = 100;

    // initial values
    latency_mean = latency_std = 0;
}

double LatencyCalculator::mean()
{
    return latency_mean;
}

double LatencyCalculator::std()
{
    return latency_std;
}

void LatencyCalculator::onNewContainerInfo(PipelineContainerInfo i)
{
    // adding new delta
    latency_ms.push_back(i.delta());

    // removing old data
    if(latency_ms.size() > frames_window)
        latency_ms.pop_front();

    // updating
    update();
}
