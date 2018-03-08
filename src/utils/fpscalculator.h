#ifndef FPSCALCULATOR_H
#define FPSCALCULATOR_H

#include <QObject>
#include <QLinkedList>

/*
 * This class calculates the frequency of frames
 * when the caller calls newFrame
 * Results can be obtained via mean() and std()
 */

class FPSCalculator : public QObject
{ Q_OBJECT
private:
    // list of values_to_keep FPS values
    QLinkedList<double> values;

    // timestamps of frames
    QLinkedList<qint64> timestamps;

    // last mean fps
    double last_mean;

    // last std fps
    double last_std;

    // number of FPS values to store
    int values_to_keep;

    // number of frames from which to calculate FPS
    int frames_window;

    // calculates mean value
    void calculateMean();

    // calculates std
    void calculateStd();

public:
    // number of last values to use for mean/std
    FPSCalculator(int to_keep = 500);

public slots:
    // called on new frame
    void newFrame();

    // obtain mean/std FPS
    double mean();
    double std();

signals:
    void newMean(double);
};

#endif // FPSCALCULATOR_H
