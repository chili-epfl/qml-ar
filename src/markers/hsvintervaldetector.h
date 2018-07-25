/**
 * @file hsvintervaldetector.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef HSVINTERVALDETECTOR_H
#define HSVINTERVALDETECTOR_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QVector2D>
#include <QColor>
#include "hsvmeancolor.h"
#include "meanstdcalculator.h"
#include "pipelinecontainer.h"

/*
 * This class receives matched points
 * and deduces the color interval they are in
 */

    /**
    * @brief Now supports only 1 color of dots
    */

class HSVIntervalDetector : public QObject
{ Q_OBJECT
private:
    /**
    * @brief Found colors
    */
    QVector<QColor> colors;

    /**
    * @brief For mean hue calculation
    */
    HSVMeanColor mean_hue;

    /**
    * @brief Minimal number of points to calculate
    * Mean and std
    */
    int min_points;

    /**
    * @brief For S, V
    */
    MeanStdCalculator mean_s, mean_v;

public:
    /**
    * @brief Minimal number of points after which emit results
    */
    HSVIntervalDetector(int min_points = 500);
    virtual ~HSVIntervalDetector() {}

public slots:
    /**
    * @brief On new set of points
    */
    void newPoints(PipelineContainer<QPair<QImage, QVector<QVector2D>>> points);

    /**
    * @brief Emits mean and intervals
    */
    void calculate();

signals:
    /**
    * @brief Returns mean and std of color
    */
    void hAvailable(double, double);
    void sAvailable(double, double);
    void vAvailable(double, double);
};

#endif // HSVINTERVALDETECTOR_H
