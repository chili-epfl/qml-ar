/**
 * @file pipelinecontainerinfo.h
 * @brief This class contains information which travels with
 * a pipeline container
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef PIPELINECONTAINERINFO_H
#define PIPELINECONTAINERINFO_H

#include <QVector>

/**
 * @brief This class contains information which travels with
 * a pipeline container
 */

class PipelineContainerInfo
{
private:
    /**
    * @brief Id of the object
    */
    int id_;

    /**
    * @brief Timestamps and stages in the pipeline
    */
    QVector<qint64> timestamps;
    QVector<QString> stages;
public:
    /**
    * @brief Initialize
    */
    PipelineContainerInfo(int id = 0);

    /**
    * @brief Add timestamp and name to vectors
    */
    void checkpoint(QString name);

    /**
     * @brief Checkpoint and return the resulting ContainerInfo
     */
    PipelineContainerInfo checkpointed(QString name);

    /**
    * @brief Last - first
    */
    qint64 delta();

    /**
    * @brief Start time
    */
    qint64 start();

    /**
    * @brief String representation
    */
    QString toString();

    /**
    * @brief Current id
    */
    int id();
};

#endif // PIPELINECONTAINERINFO_H
