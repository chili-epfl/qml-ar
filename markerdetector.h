#ifndef MARKERDETECTOR_H
#define MARKERDETECTOR_H

#include <QObject>
#include <QMatrix4x4>
#include <QString>
#include <QImage>
#include "markerstorage.h"

/*
 * This class stores a common interface for
 * marker detectors
 */

class MarkerDetector: public QObject
{ Q_OBJECT Q_PROPERTY(QMatrix4x4 camera_matrix READ getCameraMatrix NOTIFY newCameraMatrix)
public slots:
    // children call it when a new matrix should be computed
    void recomputeCameraMatrix();

protected:
    // marker positions
    MarkerStorage markers;

    // input image
    QImage input_buffer;

    // preview image
    QImage output_buffer;

    // resulting projection matrix
    QMatrix4x4 camera_matrix;

    // get initial projection matrix
    QMatrix4x4 getProjectionMatrix();

    // initial camera calibration matrix
    QMatrix3x3 camera_projection_matrix;
    virtual QVector2D getMarkerPosition(int marker_id);
public:
    MarkerDetector();

    // get marker positions from a json file
    // see MarkerStorage for an example
    void loadMarkerPositions(QString filename);

    // set input camera image
    void setInput(QImage camera);

    // do marker detection
    virtual void process() {}

    // obtain preview image after detection
    QImage getPreview();

    // obtain resulting projection matrix
    QMatrix4x4 getCameraMatrix();

    // iterators for going through the map
    QMap<int, Marker>::iterator begin();
    QMap<int, Marker>::iterator end();

    void setCameraProjectionMatrix(QMatrix3x3 mat);

signals:
    // emitted when a new projection matrix is available
    void newCameraMatrix();
};

#endif // MARKERDETECTOR_H
