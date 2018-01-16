#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector()
{
}

void MarkerDetector::setInput(QImage camera)
{
    input_buffer = camera;
}

void MarkerDetector::loadMarkerPositions(QString filename)
{
    markers.populateFromFile(filename);
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

WorldImageCorrespondences MarkerDetector::getCorrespondences()
{
    WorldImageCorrespondences result;
    result.clear();

    QMap<int, Marker>::iterator it;
    for(it = markers.begin(); it != markers.end(); it++)
        result.join((*it).getCorrespondences());

    return result;
}

bool MarkerDetector::markersDetected()
{
    return getCorrespondences().size() > 0;
}
