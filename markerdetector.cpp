#include <QCameraLens>
#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector()
{

}

void MarkerDetector::recomputeProjector()
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
        projection_matrix = getInitialProjectionMatrix() *
                (new_projection_matrix / detected_markers_n);
        emit newProjector();
    }
}

QMatrix4x4 MarkerDetector::getInitialProjectionMatrix()
{
    if(input_buffer.width() * input_buffer.height() == 0)
    {
        qDebug() << "Empty image";
        return QMatrix4x4();
    }

    Qt3DRender::QCameraLens lens;

    // for orthographic projection
    lens.setOrthographicProjection(0, input_buffer.width(), 0, input_buffer.height(), -1, 1);

    // for perspective projection
    // lens.setPerspectiveProjection(45, 1, -1, 1);
    return lens.projectionMatrix();
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

QMatrix4x4 MarkerDetector::getProjectionMatrix()
{
    return projection_matrix;
}

QMap<int, Marker>::iterator MarkerDetector::begin()
{
    return markers.begin();
}

QMap<int, Marker>::iterator MarkerDetector::end()
{
    return markers.end();
}
