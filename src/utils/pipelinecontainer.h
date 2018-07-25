/**
 * @file pipelinecontainer.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef PIPELINECONTAINER_H
#define PIPELINECONTAINER_H

#include <QObject>
#include <QDateTime>
#include "pipelinecontainerinfo.h"

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
    PipelineContainerInfo info_;
public:
    PipelineContainer(T object, PipelineContainerInfo info);
    PipelineContainer(T object);
    PipelineContainer();

    /**
    * @brief Get current id
    */
    int id();

    /**
    * @brief Get underlying object
    */
    T& o();
    operator T&();

    /**
    * @brief Info management
    */
    void setInfo(PipelineContainerInfo info);
    PipelineContainerInfo info();
    operator PipelineContainerInfo();
};

#include "pipelinecontainer.cpp"

#endif // PIPELINECONTAINER_H
