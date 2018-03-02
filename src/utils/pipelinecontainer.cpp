#ifndef PIPELINECONTAINER_CPP
#define PIPELINECONTAINER_CPP
#include <QtCore>
#include "pipelinecontainer.h"

template<class T>
PipelineContainer<T>::PipelineContainer(T object, PipelineContainerInfo info)
{
    this->object = object;
    this->info_ = info;
}

template<class T>
PipelineContainer<T>::PipelineContainer(T object)
{
    this->object = object;
}

template<class T>
PipelineContainer<T>::PipelineContainer()
{

}

template<class T>
void PipelineContainer<T>::setInfo(PipelineContainerInfo info)
{
    this->info_ = info;
}

template<class T>
PipelineContainerInfo PipelineContainer<T>::info()
{
    return info_;
}

template<class T>
PipelineContainer<T>::operator PipelineContainerInfo()
{
    return info();
}

template<class T>
PipelineContainer<T>::operator T &()
{
    return o();
}

template<class T>
T &PipelineContainer<T>::o()
{
    return object;
}

#endif
