#ifndef MARKERDETECTOR_H
#define MARKERDETECTOR_H

#include <QObject>
#include <QQuickImageProvider>
#include <QMatrix4x4>
#include <QString>
#include <QImage>
#include "markerstorage.h"

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
    void markersUpdated();

protected:
    // marker positions
    MarkerStorage markers;

    // input image
    QImage input_buffer;

    // preview image
    QImage output_buffer;
public:
    MarkerDetector();
    virtual ~MarkerDetector() {}

    // get marker positions from a json file
    // see MarkerStorage for an example
    void loadMarkerPositions(QString filename);

    // set input camera image
    void setInput(QImage camera);

    // do marker detection
    virtual void process() {}

    // obtain preview image after detection
    QImage getPreview();

    // obtain last input buffer
    QImage getLastInput();

    // iterators for going through the map
    QMap<int, Marker>::iterator begin();
    QMap<int, Marker>::iterator end();

    // return all of the 3D-2D correspondences
    WorldImageCorrespondences getCorrespondences();
};

#endif // MARKERDETECTOR_H
