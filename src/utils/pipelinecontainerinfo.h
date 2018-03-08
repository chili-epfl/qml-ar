#ifndef PIPELINECONTAINERINFO_H
#define PIPELINECONTAINERINFO_H

#include <QVector>

/*
 * This class contains information which travels with
 * a pipeline container
 */

class PipelineContainerInfo
{
private:
    // id of the object
    int id_;

    // timestamps and stages in the pipeline
    QVector<qint64> timestamps;
    QVector<QString> stages;

public:
    // initialize
    PipelineContainerInfo(int id = 0);

    // add timestamp and name to vectors
    void checkpoint(QString name);
    PipelineContainerInfo checkpointed(QString name);

    // last - first
    qint64 delta();

    // string representation
    QString toString();

    // current id
    int id();
};

#endif // PIPELINECONTAINERINFO_H
