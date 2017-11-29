#include <QCameraLens>
#include "markerdetector.h"

MarkerDetector::MarkerDetector()
{

}

void MarkerDetector::recomputeProjector()
{
    // projection_matrix = ...
    emit newProjector();
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
