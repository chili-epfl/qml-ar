#include "threadexecutor.h"
#include "config.h"

template <typename Input, typename Output, typename Processor>
ThreadExecutor<Input, Output, Processor>::ThreadExecutor(Processor* p, void (Processor::*process_)(Input*, Output*))
{
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
    this->p = p;
    this->process = process_;
}

template <typename Input, typename Output, typename Processor>
void ThreadExecutor<Input, Output, Processor>::setInput(Input *inp)
{
    // simply assigning the pointer
    input = inp;

    // telling thread to continue
    inputSem.release();
}

template <typename Input, typename Output, typename Processor>
void ThreadExecutor<Input, Output, Processor>::produce()
{
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
    (*p.*process)((Input*) input, (Output*) &(values[num_to_use]));

    // updating newest and freeing the item
    meta.lock();
    newest = num_to_use;
    used[num_to_use] -= 1;
    meta.unlock();
}

template <typename Input, typename Output, typename Processor>
int ThreadExecutor<Input, Output, Processor>::getOutputIndex()
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

template <typename Input, typename Output, typename Processor>
Output* ThreadExecutor<Input, Output, Processor>::getOutput(int index)
{
    // returning the item in the array
    return (Output*) &(values[index]);
}

template <typename Input, typename Output, typename Processor>
void ThreadExecutor<Input, Output, Processor>::freeOutput(int index)
{
    if(index < 0)
        return;
    // unlocking the output
    meta.lock();
    used[index] -= 1;
    meta.unlock();
}

template<typename Input, typename Output, typename Processor>
void ThreadExecutor<Input, Output, Processor>::run()
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
