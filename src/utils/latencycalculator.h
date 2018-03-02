#ifndef LATENCYCALCULATOR_H
#define LATENCYCALCULATOR_H

#include <QObject>
#include "pipelinecontainerinfo.h"
#include <QLinkedList>

/*
 * This class calculates latency based on PipelineContainerInfos
 */

class LatencyCalculator : public QObject
{ Q_OBJECT
private:
    int frames_window;
    QLinkedList<qint64> latency_ms;
    double latency_mean, latency_std;
    void update();
public:
    LatencyCalculator();
    double mean();
    double std();
public slots:
    void onNewContainerInfo(PipelineContainerInfo i);
};

#endif // LATENCYCALCULATOR_H
