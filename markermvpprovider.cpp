#include "markermvpprovider.h"
#include <QCameraLens>

MarkerMVPProvider::MarkerMVPProvider(MarkerDetector* d, CalibratedCamera* c)
{
    Q_ASSERT(d != NULL);
    Q_ASSERT(c != NULL);
    detector = d;
    camera = c;
    connect(detector, SIGNAL(markersUpdated()), (MarkerMVPProvider*) this, SLOT(recompute()));
}

QMatrix4x4 MarkerMVPProvider::getMV()
{
    // obtain ModelView matrix from Marker correspondences
    return detector->getCorrespondences().computePnP();
}

QMatrix4x4 MarkerMVPProvider::getP()
{
    QImage input_buffer = detector->getLastInput();
    if(input_buffer.width() * input_buffer.height() == 0)
    {
        qDebug() << "Empty image";
        return QMatrix4x4();
    }

    qDebug() << input_buffer.width() << input_buffer.height();

    /*float n = 0.01;
    float f = 10;*/

    float n = -1;
    float f = 1;

    float l = 0;
    float r = input_buffer.width();
    float b = 0;
    float t = input_buffer.height();

    Qt3DRender::QCameraLens lens;

    // for orthographic projection
    lens.setOrthographicProjection(l, r, b, t, n, f);

    // for perspective projection
    //lens.setFrustumProjection(l, r, b, t, n, f);
    return lens.projectionMatrix();

    // get matrix from the camera projection matrix (calibrated)
    return camera->getPerspectiveMatrix(n, f, l, r, b, t);
}

void MarkerMVPProvider::recompute()
{
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
        qDebug() << "RECOMPUTE";
        mvp_matrix = p * mv;
        emit newMVPMatrix();
    }
}
