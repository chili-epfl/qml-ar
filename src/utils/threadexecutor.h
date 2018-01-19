#ifndef THREADEXECUTOR_H
#define THREADEXECUTOR_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>

/*
 * This class implements producer-consumer framework
 * allowing to execute a method of a class in a separate thread
 * and to obtain the latest output in any thread
 */

// input, output types
// processor -- class which contains the method to process inputs

template <typename Input, typename Output, typename Processor>
class ThreadExecutor : public QThread
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
    volatile Output values[MAX_USERS];

    // single input for the processor
    volatile Input* input;

    // index in the values pointing to the latest one
    volatile int newest;

    // processor object
    Processor* p;

    // pointer to the function in the object
    void (Processor::*process)(Input*, Output*);
public:
    // initialize with processor and member function
    ThreadExecutor(Processor* p, void (Processor::*process_)(Input*, Output*));

    ~ThreadExecutor() {}

    // set input to this value
    void setInput(Input* inp);

    // update outputs based on last input
    void produce();

    // returns last output index
    // and blocks it from writing to
    int getOutputIndex();

    // returns output at index
    Output* getOutput(int index);

    // allows to write to this index again
    void freeOutput(int index);

public:
    // the process loop
    void run();
};

#include "threadexecutor.cpp"

#endif // THREADEXECUTOR_H
