#include "timelogger.h"
#include "marker.h"
#include "markerdetector.h"

MarkerDetector::MarkerDetector(const MarkerDetector &detector) : QObject()
{
    this->input_buffer = detector.input_buffer;
    this->markers = detector.markers;

    buffer_is_nonempty = false;
}

MarkerDetector::MarkerDetector()
{
    // handling results on thread finish
    watcher.setParent(this);
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
}

void MarkerDetector::handleFinished()
{
    // saving result from watcher
    QPair<MarkerStorage, QImage> result = watcher.result();

    // saving markers
    this->markers = result.first;
    this->output_buffer = result.second;

    PipelineContainerInfo info = object_in_process.checkpointed("MarkerDetector");

    // passing markers further
    emit markersUpdated(PipelineContainer<MarkerStorage>(markers, info));
    emit previewUpdated(PipelineContainer<QImage>(output_buffer, info));

    // starting new job if buffer is not empty
    if(buffer_is_nonempty)
    {
        buffer_is_nonempty = false;
        setInput(input_buffer);
    }
}

void MarkerDetector::setInput(PipelineContainer<QImage> camera)
{
    // saving last input
    input_buffer = camera;

    // starting thread if one is not running already
    if(!watcher.isRunning())
    {
        // saving id
        object_in_process = camera.info();

        QFuture<QPair<MarkerStorage, QImage>> future = QtConcurrent::run(this, &MarkerDetector::process, camera.o());
        watcher.setFuture(future);
    }
    // for starting job right after previous one finished
    else buffer_is_nonempty = true;
}

void MarkerDetector::loadMarkerPositions(QString filename)
{
    markers.populateFromFile(filename);
}
