#include "markermvpprovider.h"
#include "posecamerapnp.h"
#include <QCameraLens>

MarkerMVPProvider::MarkerMVPProvider(MarkerDetector* d, PerspectiveCamera* c)
{
    Q_ASSERT(d != NULL);
    Q_ASSERT(c != NULL);
    detector = d;
    camera = c;

    // recompute MVP on each markers update
    connect(detector, SIGNAL(markersUpdated()), (MarkerMVPProvider*) this, SLOT(recompute()));
}

QMatrix4x4 MarkerMVPProvider::getMV()
{
    // obtain ModelView matrix from Marker correspondences
    WorldImageCorrespondences correspondences = detector->getCorrespondences();
    Pose pose = CameraPoseEstimatorCorrespondences::estimate(camera, &correspondences);

    // if got valid pose
    if(pose.isValid())
    {
        // return 4x4 MV matrix
        return pose.get4Matrx();
    }

    return QMatrix4x4();
}

QMatrix4x4 MarkerMVPProvider::getP(double n, double f)
{
    QImage input_buffer = detector->getLastInput();
    if(input_buffer.width() * input_buffer.height() == 0)
    {
        qDebug() << "Empty image";
        return QMatrix4x4();
    }

    // get matrix from the camera projection matrix (calibrated)
    QMatrix4x4 project = camera->getPerspectiveMatrix(n, f);

    // translate to clip coordinates: [-1, 1], [-1, 1]
    QMatrix4x4 translate_m1;
    translate_m1.translate(-1, 1, 0);

    // scale to get to clip coordinates
    // y axis points UP in OpenGL -> -1
    QMatrix4x4 scaler;
    scaler.scale(2. / input_buffer.width(), -2. / input_buffer.height(), 1);

    // resulting projection
    return translate_m1 * scaler * project;
}

void MarkerMVPProvider::recompute()
{
    // coordinate systems:
    // original points from QML model (OpenGL):
    //  ---> Cellulo coordinate system (x right, y down, z to the table)
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


    // do nothing if no markers were detected
    if(detector->getCorrespondences().size() <= 0)
        return;

    // obtain Projection matrix
    QMatrix4x4 p = getP();

    // obtain ModelView matrix
    QMatrix4x4 mv = getMV();

    // calculate new MVP matrix
    QMatrix4x4 new_mvp_matrix = p * mv;

    // if it's different, save it and
    // notify listeners
    if(new_mvp_matrix != mvp_matrix)
    {
        mvp_matrix = new_mvp_matrix;
        emit newMVPMatrix();
    }
}
