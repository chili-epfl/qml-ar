/**
 * @file pipelinecontainer.h
 * @brief This template class wraps an object
 * adding additional information: id, timestamp
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef PIPELINECONTAINER_H
#define PIPELINECONTAINER_H

#include <QObject>
#include <QDateTime>
#include "pipelinecontainerinfo.h"

/**
 * @brief This template class wraps an object
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
    /**
     * @brief Initialize the container
     * @param object The object to wrap
     * @param info The additional info to store
     */
    PipelineContainer(T object, PipelineContainerInfo info);

    /**
     * @brief Initialize the container with empty info
     * @param object Thect to wrap
     */
    PipelineContainer(T object);

    /** @brief Default constructor */
    PipelineContainer();

    /**
    * @brief Get current id
    */
    int id();

    /**
    * @brief Get underlying object
    */
    T& o();

    /**
    * @brief Get underlying object
    */
    operator T&();

    /**
    * @brief Info management
    */
    void setInfo(PipelineContainerInfo info);

    /**
     * @brief Get current info
     */
    PipelineContainerInfo info();

    /**
     * @brief Get current info
     */
    operator PipelineContainerInfo();
};

#include "pipelinecontainer.cpp"

#endif // PIPELINECONTAINER_H
