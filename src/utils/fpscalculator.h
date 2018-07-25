/**
 * @file fpscalculator.h
 * @brief This class calculates the frequency of frames
 * when the caller calls newFrame
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef FPSCALCULATOR_H
#define FPSCALCULATOR_H

#include <QObject>
#include <QLinkedList>

/**
 * @brief This class calculates the frequency of frames
 * when the caller calls newFrame
 *
 * Results can be obtained via mean() and std()
 */

class FPSCalculator : public QObject
{ Q_OBJECT
private:
    /**
    * @brief List of values_to_keep FPS values
    */
    QLinkedList<double> values;

    /**
    * @brief Timestamps of frames
    */
    QLinkedList<qint64> timestamps;

    /**
    * @brief Last mean fps
    */
    double last_mean;

    /**
    * @brief Last std fps
    */
    double last_std;

    /**
    * @brief Number of FPS values to store
    */
    int values_to_keep;

    /**
    * @brief Number of frames from which to calculate FPS
    */
    int frames_window;

    /**
    * @brief Calculates mean value
    */
    void calculateMean();

    /**
    * @brief Calculates std
    */
    void calculateStd();

public:
    // number of last values to use for mean/std
    FPSCalculator(int to_keep = 500);

public slots:
    /**
    * @brief Called on new frame
    */
    void newFrame();

    // obtain mean/std FPS
    double mean();
    double std();

signals:
    void newMean(double);
};

#endif // FPSCALCULATOR_H
