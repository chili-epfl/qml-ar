#include "pipelinethread.h"
#include "config.h"
#include "threadworker.h"

PipelineThread::PipelineThread(ThreadWorker* worker)
{
    Q_ASSERT(worker != NULL);

    // initially, nothing is used
    for(int i = 0; i < MAX_USERS; i++)
    {
        used[i] = 0;
    }

    // no input
    input = NULL;

    // no newest item
    newest = -1;

    // saving object and method ptrs
    source = NULL;
    destination = NULL;
    this->worker = worker;
}

void PipelineThread::update()
{
    // telling thread to continue
    inputSem.release();
}

void PipelineThread::setSource(PipelineThread* source)
{
    this->source = source;
}

void PipelineThread::setDestination(PipelineThread* destination)
{
    this->destination = destination;
}

void PipelineThread::setInput(PipelineElement *inp)
{
    // simply assigning the pointer
    input = inp;

    source->inputSem.release();
}

void PipelineThread::produce()
{
    int source_index = -1;
    if(source != NULL)
    {
        source_index = source->getOutputIndex();
        if(source_index == -1)
            return;
        input = source->getOutput(source_index);
    }

    // not processing null input
    if(input == NULL)
        return;

    // critical section
    meta.lock();

    // obtaining free item
    int num_to_use = -1;
    for(int i = 0; i < MAX_USERS; i++)
        if(!used[i] && i != newest)
        {
            num_to_use = i;
            break;
        }

    // trying not to use the newest
    if(newest >= 0 && num_to_use == -1 && !used[newest])
        num_to_use = newest;

    if(num_to_use == -1)
    {
        TimeLoggerLog("%s", "Error producing");
        return;
    }

    // using the item
    used[num_to_use] += 1;
    meta.unlock();

    // processing input and storing the result
    worker->threadIteration((PipelineElement*) input, (PipelineElement*) &(values[num_to_use]));

    // updating newest and freeing the item
    meta.lock();
    newest = num_to_use;
    used[num_to_use] -= 1;
    meta.unlock();

    if(source != NULL)
    {
        source->freeOutput(source_index);
    }

    if(destination != NULL)
        destination->update();
}

int PipelineThread::getOutputIndex()
{
    int num_to_read = -1;

    // obtaining newest and locking it
    meta.lock();
    num_to_read = newest;
    if(num_to_read >= 0)
        used[num_to_read] += 1;
    meta.unlock();

    // no result
    if(num_to_read == -1)
    {
        TimeLoggerLog("%s", "No data available");
    }

    return num_to_read;
}

PipelineElement* PipelineThread::getOutput(int index)
{
    // returning the item in the array
    return (PipelineElement*) &(values[index]);
}

void PipelineThread::freeOutput(int index)
{
    if(index < 0)
        return;
    // unlocking the output
    meta.lock();
    used[index] -= 1;
    meta.unlock();
}

void PipelineThread::run()
{
    TimeLoggerLog("%s", "Starting background thread");
    while(true)
    {
        int available = inputSem.available();
        for(int i = 0; i < available; i++)
            inputSem.acquire();
        inputSem.acquire();

        TimeLoggerLog("%s", "Producing new output");

        produce();
    }
}
