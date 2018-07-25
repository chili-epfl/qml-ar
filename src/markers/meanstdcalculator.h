/**
 * @file meanstdcalculator.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

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
    /**
    * @brief Values to calculate
    */
    QVector<double> values;
public:
    MeanStdCalculator();

    /**
    * @brief Add value
    */
    void addValue(double value);

    /**
    * @brief Returns mean
    */
    double getMean();

    /**
    * @brief Returns std
    */
    double getStd();
};

#endif // MEANSTDCALCULATOR_H
