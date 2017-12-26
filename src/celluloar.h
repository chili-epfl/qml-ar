#ifndef CELLULOAR_H
#define CELLULOAR_H

#include <QtCore>
#include <QtQml>
#include <QString>
#include <QQuickImageProvider>
#include "uchiyamarkerdetector.h"
#include "imagebackend.h"
#include "perspectivecamera.h"
#include "portablebackendfactory.h"
#include "markerbackend.h"
#include "markermvpprovider.h"
#include "calibratedcamerafilestorage.h"

/*
 * This class is the C++/QML interface to the
 * Cellulo AR library
 */

class CelluloAR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)

public:
    // empty constructor
    // object must be initialized with
    // one of the methods below
    CelluloAR();

    // returns camera id to be used
    // as raw input
    int getCameraId();

    QString getImageFilename();

public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // returns resulting MVP matrix
    QMatrix4x4 getMVPMatrix();

    // call to emit newMVPMatrix signal
    void newMVPMatrixSlot();

    // get processed image provider
    QQuickImageProvider* getImageProvider();

private:
    // name of the image to be used
    // as raw input
    QString image_filename;

    // camera id to be used
    // as raw input
    int camera_id;

    // true iff can obtain MVP matrix
    bool is_initialized;

    // instantiate objects
    void initialize();

    // image provider (camera)
    QQuickImageProvider* raw_provider;

    // marker detector
    UchiyaMarkerDetector* detector;

    // camera matrix
    CalibratedCamera* camera_matrix;

    // perspective camera model
    PerspectiveCamera* perspective_camera;

    // marker backend for visualizing
    // detected markers
    MarkerBackEnd* marker_backend;

    // ModelViewProjection matrix
    // provider
    MVPProvider* mvp_provider;

signals:
    // notify QML part when new matrix is available
    void newMVPMatrix();
};

#endif // CELLULOAR_H
