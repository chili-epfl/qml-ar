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

    // https://www.ibm.com/support/knowledgecenter/ssw_aix_72/com.ibm.aix.opengl/doc/openglrf/glOrtho.htm
    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glOrtho.xml

    float l = 0, r = input_buffer.width(), bot = 0, top = input_buffer.height(), near = -1, far = 1;

    float tx = -(r + l)/(r - l);
    float ty = -(top + bot) / (top - bot);
    float tz = -(far + near) / (far - near);

    const float a[] = {2 / (r - l), 0.0f, 0.0f, tx,
                       0.0f, 2 / (top - bot), 0.0f, ty,
                       0.0f, 0.0f, -2/(far-near), tz,
                       0.0f, 0.0f, 0.0f, 1.0f};

    return QMatrix4x4(a);
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
