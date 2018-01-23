#include "markermvpprovider.h"
#include "posecamerapnp.h"
#include <QCameraLens>
#include "timelogger.h"
#include "config.h"

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
    scaler.scale(2. / camera->width(), -2. / camera.height(), 1);

    // resulting projection
    return translate_m1 * scaler * project;
}

void MarkerMVPProvider::recompute(MarkerStorage storage)
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

    // hide objects if no markers were detected
    if(storage.getCorrespondences().size() <= 0)
    {
        reset();
        return;
    }

    // obtain Projection matrix
    p_matrix = getP();

    // if P matrix is invalid
    if(p_matrix.isIdentity())
    {
        // hide all objects
        reset();
        return;
    }

    // obtain ModelView matrix
    mv_matrix = getMV(storage);

    // calculate new MVP matrix
    QMatrix4x4 new_mvp_matrix = p_matrix * mv_matrix;

    // notify listeners
    setMVPMatrix(new_mvp_matrix);

    TimeLoggerProfile("%s", "Calculated MVP matrix");
}
