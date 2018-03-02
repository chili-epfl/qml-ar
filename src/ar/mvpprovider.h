#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>
#include "pipelinecontainer.h"

/*
 * This class represents a QML interface
 * to provide ModelViewProjection matrix
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

class MVPProvider : public QObject
{ Q_OBJECT
protected:
    // last ModelViewProjection matrix
    QMatrix4x4 mvp_matrix;

    // last ModelView matrix
    QMatrix4x4 mv_matrix;

    // last ModelViewProjection matrix
    QMatrix4x4 p_matrix;

    // current input id
    PipelineContainerInfo object_in_process;

    // set MVP matrix internally
    void setMVPMatrix(QMatrix4x4 mat);
public:
    // initialize with eye matrix
    MVPProvider();

    // invalidate matrix
    // makes all objects invisible
    void reset();

    // true if result is an actual matrix
    static bool isValid(QMatrix4x4 mat);
signals:

    // emitted when new MVP matrix is available
    void newMVPMatrix(PipelineContainer<QMatrix4x4>);
    void newPMatrix(PipelineContainer<QMatrix4x4>);
    void newMVMatrix(PipelineContainer<QMatrix4x4>);
    void newInfo(PipelineContainerInfo);
};

#endif // MVPPROVIDER_H
