#ifndef PIPELINECONTAINER_CPP
#define PIPELINECONTAINER_CPP
#include <QtCore>
#include "pipelinecontainer.h"

template<class T>
PipelineContainer<T>::PipelineContainer(T object, int id)
{
    this->object = object;
    this->id_ = id;
    this->creation_timestamp = QDateTime::currentMSecsSinceEpoch();
}

template<class T>
PipelineContainer<T>::PipelineContainer()
{
    id_ = -1;
}

template<class T>
int PipelineContainer<T>::id()
{
    Q_ASSERT(id_ >= 0);
    return id_;
}

template<class T>
T &PipelineContainer<T>::o()
{
    Q_ASSERT(id_ >= 0);
    return object;
}

template<class T>
PipelineContainer<T>::operator T &()
{
    return o();
}

#endif
