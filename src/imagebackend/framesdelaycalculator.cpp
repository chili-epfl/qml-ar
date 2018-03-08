#include "framesdelaycalculator.h"

FramesDelayCalculator::FramesDelayCalculator()
{
    // some initial values
    mean_latency = 0;
    mean_fps = 30;
    result = 0;
}

int FramesDelayCalculator::lastDelay()
{
    return result;
}

int FramesDelayCalculator::getDelay()
{
    // rewinding back to proper timing
    int delay_in_frames = qRound(mean_latency * mean_fps / 1000.);
    return delay_in_frames;
}

void FramesDelayCalculator::update()
{
    result = getDelay();
    emit newDelay(result);
}

void FramesDelayCalculator::setMeanLatency(double lat_ms)
{
    mean_latency = lat_ms;
    update();
}

void FramesDelayCalculator::setMeanFPS(double fps_ms)
{
    mean_fps = fps_ms;
    update();
}
