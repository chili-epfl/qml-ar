/**
 * @file framesdelaycalculator.h
 * @brief This class calculates the delay in frames based on desired latency and
 * actual FPS
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef FRAMESDELAYCALCULATOR_H
#define FRAMESDELAYCALCULATOR_H

/**
 * @brief This class calculates the delay in frames based on desired latency and
 * actual FPS
 */

#include <QObject>

class FramesDelayCalculator : public QObject
{ Q_OBJECT
public:
    /** @brief Constructor */
    FramesDelayCalculator();

    /**
    * @brief Returns last value
    */
    int lastDelay();

public slots:
    /**
     * @brief setMeanLatency
     * @param lat_ms Mean latency in ms
     */
    void setMeanLatency(double lat_ms);

    /**
     * @brief setMeanFPS
     * @param fps_ms Mean fps
     */
    void setMeanFPS(double fps_ms);

private:
    /**
    * @brief Mean latency from camera to markers
    */
    double mean_latency;

    /**
     * @brief Mean FPS from camera to markers
     */
    double mean_fps;

    /**
    * @brief Result in frames
    */
    int result;

    /**
    * @brief Update local result
    */
    void update();

    /**
    * @brief Returns delay in frames
    */
    int getDelay();

signals:
    /**
     * @brief newDelay
     * @param delay new delay in frames
     */
    void newDelay(int delay);
};

#endif // FRAMESDELAYCALCULATOR_H
