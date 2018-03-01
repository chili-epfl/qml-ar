#ifndef MARKERDETECTOR_H
#define MARKERDETECTOR_H

#include <QObject>
#include <QQuickImageProvider>
#include <QMatrix4x4>
#include <QString>
#include <QImage>
#include "markerstorage.h"
#include "pipelinecontainer.h"
#include <QtConcurrent>

/*
 * This class stores a common interface for
 * marker detectors
 *
 * INPUT/OUTPUT COORDINATE SYSTEM (OpenCV/QML):
 * x right
 * y down
 * z from reader
 */

class MarkerDetector: public QObject
{ Q_OBJECT
signals:
    // children call it when a new matrix should be computed
    void markersUpdated(MarkerStorage);

    // called on new preview image
    void previewUpdated(QImage);

public slots:
    // set input camera image
    void setInput(PipelineContainer<QImage> camera);

    // called on new result from thread
    void handleFinished();

protected:
    // marker positions
    MarkerStorage markers;

private:

    // input image
    QImage input_buffer;

    // preview image
    QImage output_buffer;

    // true if input contains unprocessed data
    int buffer_is_nonempty;

    // for background detection
    QFutureWatcher<QPair<MarkerStorage, QImage>> watcher;

public:
    MarkerDetector();
    MarkerDetector(const MarkerDetector &detector);
    virtual ~MarkerDetector() {}

    // get marker positions from a json file
    // see MarkerStorage for an example
    void loadMarkerPositions(QString filename);

    // do marker detection
    virtual QPair<MarkerStorage, QImage> process(QImage img) = 0;
};

#endif // MARKERDETECTOR_H
