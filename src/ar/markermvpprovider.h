/**
 * @file markermvpprovider.h
 * @brief This class implements MVPProvider
 * using pose from a detected markers,
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef MARKERMVPPROVIDER_H
#define MARKERMVPPROVIDER_H

#include "mvpprovider.h"
#include "markerdetector.h"
#include "perspectivecamera.h"

/**
 * @brief This class implements MVPProvider
 * using pose from a detected markers,
 * a calibrated camera's matrix
 * multiplies them and outputs MVP
 *
 * INPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 *
 * OUTPUT COORDINATE SYSTEM (OpenGL)
 * x right
 * y up
 * z to reader
 *
 */

class MarkerMVPProvider : public MVPProvider
{ Q_OBJECT
private:
    PerspectiveCamera* camera;

    /**
    * @brief Get perspective matrix P
    * @param n Near, in mm (>0)
    * @param f Far, in mm (>0)
    */
    QMatrix4x4 getP(double n = 1, double f = 10000);

    /**
    * @brief Get ModelView matrix MV
    * @param storage Detected markers to process
    */

    QMatrix4x4 getMV(MarkerStorage storage);

public:

    /**
    * @brief Initialize using a perspective camera
    * @param c PerspectiveCamera object pointer
    */
    MarkerMVPProvider(PerspectiveCamera *c);

public slots:
    /**
    * @brief This function actually compute matrices
    * @param storage Detected markers to process
    */
    void recompute(PipelineContainer<MarkerStorage> storage);
};

#endif // MARKERMVPPROVIDER_H

