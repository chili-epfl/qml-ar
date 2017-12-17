#include "markermvpprovider.h"
#include <QCameraLens>

MarkerMVPProvider::MarkerMVPProvider(MarkerDetector* d, CalibratedCamera* c)
{
    Q_ASSERT(d != NULL);
    Q_ASSERT(c != NULL);
    detector = d;
    camera = c;
    connect(detector, SIGNAL(markersUpdated()), this, SLOT(recompute()));
}

QMatrix4x4 MarkerMVPProvider::getMV()
{
    return QMatrix4x4();
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
    QMatrix4x4 p = getP();
    QMatrix4x4 mv = getMV();
    mvp_matrix = p * mv;
    emit newMVPMatrix();
}
