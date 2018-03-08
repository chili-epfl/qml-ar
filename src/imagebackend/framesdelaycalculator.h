#ifndef FRAMESDELAYCALCULATOR_H
#define FRAMESDELAYCALCULATOR_H

/*
 * This class calculates the delay in frames based on desired latency and
 * actual FPS
 */

#include <QObject>

class FramesDelayCalculator : public QObject
{ Q_OBJECT
public:
    FramesDelayCalculator();

    // returns last value
    int lastDelay();

public slots:
    void setMeanLatency(double lat_ms);
    void setMeanFPS(double fps_ms);

private:
    // mean latency from camera to markers
    double mean_latency;
    double mean_fps;

    // result in frames
    int result;

    // update local result
    void update();

    // returns delay in frames
    int getDelay();

signals:
    void newDelay(int);
};

#endif // FRAMESDELAYCALCULATOR_H
