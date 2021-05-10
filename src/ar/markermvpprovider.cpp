/**
 * @file markermvpprovider.cpp
 * @brief This class implements MVPProvider
 * using pose from a detected markers,
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "markermvpprovider.h"
#include "posecamerapnp.h"
#include <QCameraLens>
#include "timelogger.h"
#include <QDebug>

MarkerMVPProvider::MarkerMVPProvider(PerspectiveCamera* c) : MVPProvider()
{
    Q_ASSERT(c != NULL);

    // save detector and camera
    camera = c;
}

QMatrix4x4 MarkerMVPProvider::getMV(MarkerStorage storage)
{
    // obtain ModelView matrix from Marker correspondences
    WorldImageCorrespondences correspondences = storage.getCorrespondences();
    Pose pose = CameraPoseEstimatorCorrespondences::estimate(camera, &correspondences);

    // if got valid pose
    if(pose.isValid())
    {
        // return 4x4 MV matrix
        return pose.get4Matrix();
    }

    return QMatrix4x4();
}

QMatrix4x4 MarkerMVPProvider::getP(double n, double f)
{
    // get matrix from the camera projection matrix (calibrated)
    QMatrix4x4 project = camera->getPerspectiveMatrix(n, f);

    // translate to clip coordinates: [-1, 1], [-1, 1]
    QMatrix4x4 translate_m1;
    translate_m1.translate(-1, 1, 0);

    // scale to get to clip coordinates
    // y axis points UP in OpenGL -> -1
    QMatrix4x4 scaler;
    scaler.scale(2. / camera->width(), -2. / camera->height(), 1);

    // resulting projection
    return translate_m1 * scaler * project;
}

void MarkerMVPProvider::recompute(PipelineContainer<MarkerStorage> storage)
{
    // coordinate systems:
    // original points from QML model (OpenGL):
    //  ---> QML coordinate system (x right, y down, z to the table)
    //                                  origin at top-left corner of the sheet
    // after ModelView matrix
    //  ---> OpenCV coordinate system (x right, y down, z from reader)
    //                                 origin at the camera, z parallel to camera axiss
    //
    // after P matrix
    //  ---> OpenGL clip coordinates -1 <= x, y, z <= 1, w > 0, x right, y up,
    //                                      -z to computer screen
    //
    // NOTICE: MV transform is done in world space, not in OpenGL space
    //         since MV is used before P
    //

    // obtaining input id
    object_in_process = storage.info();

    // hide objects if no markers were detected
    if(storage.o().getCorrespondences().size() <= 0)
    {
        reset();
        return;
    }

    // obtain Projection matrix
    p_matrix = getP();

#ifdef DEBUG_SHADER
    qDebug() << "P matrix" << p_matrix;
#endif

    // if P matrix is invalid
    if(p_matrix.isIdentity())
    {
        // hide all objects
        reset();
        return;
    }

    // obtain ModelView matrix
    mv_matrix = getMV(storage);

#ifdef DEBUG_SHADER
    qDebug() << "MV matrix" << mv_matrix;
#endif

    // calculate new MVP matrix
    QMatrix4x4 new_mvp_matrix = p_matrix * mv_matrix;

#ifdef DEBUG_SHADER
    qDebug() << "MVP matrix" << new_mvp_matrix;
#endif

    // notify listeners
    setMVPMatrix(new_mvp_matrix);

    TimeLoggerThroughput("%s", "Calculated MVP matrix");
}
