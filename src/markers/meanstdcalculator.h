#ifndef MEANSTDCALCULATOR_H
#define MEANSTDCALCULATOR_H

#include <QObject>
#include <QVector>

/*
 * This class calculates mean and std values
 */

class MeanStdCalculator : public QObject
{ Q_OBJECT
private:
    // values to calculate
    QVector<double> values;
public:
    MeanStdCalculator();

    // add value
    void addValue(double value);

    // returns mean
    double getMean();

    // returns std
    double getStd();
};

#endif // MEANSTDCALCULATOR_H
