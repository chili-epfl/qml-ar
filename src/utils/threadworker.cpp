#include "threadworker.h"
#include "pipelinethread.h"

ThreadWorker::ThreadWorker()
{
    pipelineThread = NULL;
}

PipelineThread *ThreadWorker::getPipeline()
{
    return pipelineThread;
}

void ThreadWorker::startThread()
{
    if(pipelineThread != NULL)
        pipelineThread->start();
}
