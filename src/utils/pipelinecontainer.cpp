#include "pipelinecontainer.h"

template<class T>
PipelineContainer<T>::PipelineContainer(T object, int id)
{
    this->object = object;
    this->id = id;
    this->creation_timestamp = QDateTime::currentMSecsSinceEpoch();
}

template<class T>
T &PipelineContainer<T>::o()
{
    return object;
}

template<class T>
void PipelineContainer<T>::replaceObject(T object_new)
{
    this->object = object_new;
}
