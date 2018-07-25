/**
 * @file latencycalculator.h
 * @brief This class calculates latency based on PipelineContainerInfos
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef LATENCYCALCULATOR_H
#define LATENCYCALCULATOR_H

#include <QObject>
#include "pipelinecontainerinfo.h"
#include <QLinkedList>

/**
 * @brief This class calculates latency based on PipelineContainerInfos
 */

class LatencyCalculator : public QObject
{ Q_OBJECT
private:
    /**
    * @brief Averaging window
    */
    int frames_window;

    /**
    * @brief Latency array
    */
    QLinkedList<qint64> latency_ms;

    // mean/std local copy
    double latency_mean, latency_std;

    /**
    * @brief Call on new container
    */
    void update();

public:
    LatencyCalculator();

    /**
    * @brief Result
    */
    double mean();
    double std();

public slots:
    /**
    * @brief Called on each processed container
    */
    void onNewContainerInfo(PipelineContainerInfo i);

signals:
    /**
    * @brief Tells current mean value
    */
    void newMean(double);
};

#endif // LATENCYCALCULATOR_H
