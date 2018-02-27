#ifndef PIPELINECONTAINER_H
#define PIPELINECONTAINER_H

#include <QObject>
#include <QDateTime>

template<class T> class PipelineContainer
{
private:
    T object;
    qint64 creation_timestamp;
    int id;
public:
    PipelineContainer(T object, int id = 0);

    // get underlying object
    T& o();

    void replaceObject(T object_new);
};

#endif // PIPELINECONTAINER_H
