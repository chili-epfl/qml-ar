#ifndef PIPELINECONTAINER_H
#define PIPELINECONTAINER_H

#include <QObject>
#include <QDateTime>

/*
 * This template class wraps an object
 * adding additional information: id, timestamp
 *
 * Designed to be used in a pipeline of Qt objects
 */

template<class T> class PipelineContainer
{
private:
    T object;
    qint64 creation_timestamp;
    int id_;
public:
    PipelineContainer(T object, int id_ = 0);
    PipelineContainer();

    // get current id
    int id();

    // get underlying object
    T& o();
    operator T&();
};

#include "pipelinecontainer.cpp"

#endif // PIPELINECONTAINER_H
