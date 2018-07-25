/**
 * @file mvpprovider.h
 * @brief This class represents a QML interface
 * to provide ModelViewProjection matrix
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef MVPPROVIDER_H
#define MVPPROVIDER_H

#include <QMatrix4x4>
#include <QObject>
#include "pipelinecontainer.h"

/**
 * @brief This class represents a QML interface
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
    /**
    * @brief Last ModelViewProjection matrix
    */
    QMatrix4x4 mvp_matrix;

    /**
    * @brief Last ModelView matrix
    */
    QMatrix4x4 mv_matrix;

    /**
    * @brief Last ModelViewProjection matrix
    */
    QMatrix4x4 p_matrix;

    /**
    * @brief Current input id
    */
    PipelineContainerInfo object_in_process;

    /**
    * @brief Set MVP matrix internally
    */
    void setMVPMatrix(QMatrix4x4 mat);
public:
    /**
    * @brief Initialize with eye matrix
    */
    MVPProvider();

    /**
    * @brief Invalidate matrix
    * Makes all objects invisible
    */
    void reset();

    /**
    * @brief True if result is an actual matrix
    * @param mat Matrix to check
    */
    static bool isValid(QMatrix4x4 mat);
signals:

    /**
    * @brief Emitted when new MVP matrix is available
    * @param I resulting matrix
    */
    void newMVPMatrix(PipelineContainer<QMatrix4x4>);

    /**
    * @brief Emitted when new MVP matrix is available
    * @param I resulting matrix
    */

    void newPMatrix(PipelineContainer<QMatrix4x4>);

    /**
    * @brief Emitted when new MVP matrix is available
    * @param I resulting matrix
    */

    void newMVMatrix(PipelineContainer<QMatrix4x4>);

    /**
    * @brief Emitted when new info is available
    * @param I resulting timing info
    */

    void newInfo(PipelineContainerInfo);
};

#endif // MVPPROVIDER_H
