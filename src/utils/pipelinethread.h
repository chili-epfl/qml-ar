#ifndef THREADEXECUTOR_H
#define THREADEXECUTOR_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <QDateTime>
#include "pipelineelement.h"

class ThreadWorker;

/*
 * This class implements producer-consumer framework
 * allowing to execute a method of a class in a separate thread
 * and to obtain the latest output in any thread
 */

// input, output types
// processor -- class which contains the method to process inputs

class PipelineThread : public QThread
{
private:
    // maximal number of producers/consumers
    static const int MAX_USERS = 10;

    // mutex for accessing used[] indicators
    QMutex meta;

    // mutex for waiting for input
    QSemaphore inputSem;

    // indicates if the value is being written/read
    volatile bool used[MAX_USERS];

    // array of values for outputs
    volatile PipelineElement values[MAX_USERS];

    // single input for the processor
    volatile PipelineElement* input;

    // index in the values pointing to the latest one
    volatile int newest;

    // processor object
    PipelineThread* source;

    // receiver object
    PipelineThread* destination;

    // worker object
    ThreadWorker* worker;
public:
    // initialize with processor and member function
    PipelineThread(ThreadWorker* worker);

    ~PipelineThread() {}

    // update from source
    void update();

    // set input to this value
    void setInput(PipelineElement* inp);

    // update outputs based on last input
    void produce();

    // returns last output index
    // and blocks it from writing to
    int getOutputIndex();

    // returns output at index
    PipelineElement* getOutput(int index);

    // allows to write to this index again
    void freeOutput(int index);

    // set output manually
    void addOutput(PipelineElement* out);

public:
    // the process loop
    void run();
    void setDestination(PipelineThread *destination);
    void setSource(PipelineThread *source);
};

#include "pipelinethread.cpp"

#endif // THREADEXECUTOR_H
