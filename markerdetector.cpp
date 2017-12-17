#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector(QQuickImageProvider *provider)
{
    Q_ASSERT(provider != NULL);
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

QImage MarkerDetector::getLastInput()
{
    return input_buffer;
}

QMap<int, Marker>::iterator MarkerDetector::begin()
{
    return markers.begin();
}

QMap<int, Marker>::iterator MarkerDetector::end()
{
    return markers.end();
}
