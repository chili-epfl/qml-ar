#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector(MarkerDetector &detector)
{
    this->input_buffer = detector.input_buffer;
    this->markers = detector.markers;
    this->output_buffer_background = detector.output_buffer_background;
}

MarkerDetector::MarkerDetector()
{
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleResults()));
}

void MarkerDetector::handleResults()
{
    MarkerStorage result = watcher.result();
    emit markersUpdated(corr);
}

void MarkerDetector::setInput(QImage camera)
{
    input_buffer = camera;
    if(!watcher.isRunning())
    {
        QFuture<MarkerStorage> future = QtConcurrent::run(*this, MarkerDetector::process, input_buffer);
        watcher.setFuture(future);
    }
}

void MarkerDetector::setPreviewBackground(QImage preview)
{
    output_buffer_background = preview;
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
