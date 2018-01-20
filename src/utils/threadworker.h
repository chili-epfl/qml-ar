#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QtCore>
#include "pipelineelement.h"

class PipelineThread;

/*
 * This base class is for implementing workers for the PipelineThread
 */

class ThreadWorker
{
public:
    // thread to work on
    PipelineThread* pipelineThread;

    ThreadWorker();

    // one iteration inside the thread
    virtual void threadIteration(PipelineElement* input, PipelineElement* output) {Q_UNUSED(input) Q_UNUSED(output)}

    // returns the working thread
    PipelineThread* getPipeline();

    // starts the thread
    void startThread();
};

#endif // THREADWORKER_H
