/**
 * @file markerdetector.h
 * @brief This class stores a common interface for
 * marker detectors
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

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

/**
 * @brief This class stores a common interface for
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
    /**
    * @brief Children call it when a new matrix should be computed
    */
    void markersUpdated(PipelineContainer<MarkerStorage>);

    /**
    * @brief Called on new preview image
    */
    void previewUpdated(PipelineContainer<QImage>);

public slots:
    /**
    * @brief Set input camera image
    */
    void setInput(PipelineContainer<QImage> camera);

    /**
    * @brief Called on new result from thread
    */
    void handleFinished();

protected:
    /**
    * @brief Marker positions
    */
    MarkerStorage markers;

    /**
    * @brief Input id
    */
    PipelineContainerInfo object_in_process;

private:
    /**
    * @brief Input image
    */
    PipelineContainer<QImage> input_buffer;

    /**
    * @brief Preview image
    */
    QImage output_buffer;

    /**
    * @brief True if input contains unprocessed data
    */
    int buffer_is_nonempty;

    /**
    * @brief For background detection
    */
    QFutureWatcher<QPair<MarkerStorage, QImage>> watcher;

public:
    MarkerDetector();
    MarkerDetector(const MarkerDetector &detector);
    virtual ~MarkerDetector() {}

    /**
    * @brief Get marker positions from a json file
    * See MarkerStorage for an example
    */
    void loadMarkerPositions(QString filename);

    /**
    * @brief Do marker detection
    */
    virtual QPair<MarkerStorage, QImage> process(QImage img) = 0;
};

#endif // MARKERDETECTOR_H
