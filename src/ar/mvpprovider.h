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
    * @param mvp resulting matrix
    */
    void newMVPMatrix(PipelineContainer<QMatrix4x4> mvp);

    /**
    * @brief Emitted when new P matrix is available
    * @param p resulting matrix
    */

    void newPMatrix(PipelineContainer<QMatrix4x4> p);

    /**
    * @brief Emitted when new MV matrix is available
    * @param mv resulting matrix
    */

    void newMVMatrix(PipelineContainer<QMatrix4x4> mv);

    /**
    * @brief Emitted when new info is available
    * @param info resulting timing info
    */

    void newInfo(PipelineContainerInfo info);
};

#endif // MVPPROVIDER_H
