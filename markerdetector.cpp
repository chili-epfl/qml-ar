#include <QCameraLens>
#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector()
{

}

void MarkerDetector::recomputeCameraMatrix()
{
    QMap<int, Marker>::iterator it;

    QMatrix4x4 new_projection_matrix;
    new_projection_matrix.fill(0.0f);
    int detected_markers_n = 0;

    for(it = markers.begin(); it != markers.end(); it++)
    {
        if(!(*it).getH().isIdentity())
        {
            new_projection_matrix += (*it).getH();
            detected_markers_n    += 1;
        }
    }

    if(detected_markers_n > 0)
    {
        //qDebug() << getProjectionMatrix();

        QMatrix4x4 homography_projection_matrix = (new_projection_matrix / detected_markers_n);

        camera_matrix = getProjectionMatrix() * homography_projection_matrix;

        qDebug() << homography_projection_matrix.map(QVector4D(105, 148, 0, 1))
                << homography_projection_matrix.map(QVector4D(210, 297, 0, 1))
                << homography_projection_matrix.map(QVector4D(210, 297, 100, 1));

        emit newCameraMatrix();
    }
}

QMatrix4x4 MarkerDetector::getProjectionMatrix()
{
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
    // see http://kgeorge.github.io/2014/03/08/calculating-opengl-perspective-matrix-from-opencv-intrinsic-matrix
    float alpha = camera_projection_matrix(0, 0);
    float beta  = camera_projection_matrix(1, 1);
    float c_x   = camera_projection_matrix(0, 2);
    float c_y   = camera_projection_matrix(1, 2);

    QMatrix4x4 res;
    res(0, 0) = alpha / c_x;
    res(1, 1) = beta / c_y;
    res(2, 2) = -(f + n)/(f - n);
    res(3, 3) = 0;
    res(2, 3) = -2 * f * n / (f - n);
    res(3, 2) = -1;
    res(0, 2) = (r + l) / (r - l);
    res(1, 2) = (t + b) / (t - b);

    return res.transposed();
}

void MarkerDetector::setInput(QImage camera)
{
    input_buffer = camera;
}

void MarkerDetector::loadMarkerPositions(QString filename)
{
    markers.populateFromFile(filename);
}

QVector2D MarkerDetector::getMarkerPosition(int marker_id)
{ Q_UNUSED(marker_id)
    return QVector2D(0, 0);
}

QImage MarkerDetector::getPreview()
{
    return output_buffer;
}

QMatrix4x4 MarkerDetector::getCameraMatrix()
{
    return camera_matrix;
}

QMap<int, Marker>::iterator MarkerDetector::begin()
{
    return markers.begin();
}

QMap<int, Marker>::iterator MarkerDetector::end()
{
    return markers.end();
}

void MarkerDetector::setCameraProjectionMatrix(QMatrix3x3 mat)
{
    camera_projection_matrix = mat;
}
