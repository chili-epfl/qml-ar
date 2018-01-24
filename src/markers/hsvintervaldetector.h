#ifndef HSVINTERVALDETECTOR_H
#define HSVINTERVALDETECTOR_H

#include <QObject>
#include <QImage>
#include <QVector>
#include <QVector2D>
#include <QColor>
#include "hsvmeancolor.h"
#include "meanstdcalculator.h"

/*
 * This class receives matched points
 * and deduces the color interval they are in
 */

// now supports only 1 color of dots

class HSVIntervalDetector : public QObject
{ Q_OBJECT
private:
    // found colors
    QVector<QColor> colors;

    // for mean hue calculation
    HSVMeanColor mean_hue;

    // minimal number of points to calculate
    // mean and std
    int min_points;

    // for S, V
    MeanStdCalculator mean_s, mean_v;

public:
    // minimal number of points after which emit results
    HSVIntervalDetector(int min_points = 500);
    virtual ~HSVIntervalDetector() {}

public slots:
    // on new set of points
    void newPoints(QPair<QImage, QVector<QVector2D>> points);

    // emits mean and intervals
    void calculate();

signals:
    // returns mean and std of color
    void hAvailable(double, double);
    void sAvailable(double, double);
    void vAvailable(double, double);
};

#endif // HSVINTERVALDETECTOR_H
