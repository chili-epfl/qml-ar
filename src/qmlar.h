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
#include "imumvpdecorator.h"
#include "posepredictor.h"
#include "trackingdecorator.h"

/*
 * This class is the C++/QML interface to the
 * QML AR library
 */

class QMLAR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QObject* qml_camera READ getQMLCamera WRITE setQMLCamera)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(double update_ms READ getUpdateMS WRITE setUpdateMS)
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)

public:
    enum InitType
    {
        INIT_CAMERA,
        INIT_IMAGE,
        INIT_QMLCAMERA
    };
    Q_ENUMS(InitType)

    // empty constructor
    // object must be initialized with
    // one of the methods below
    QMLAR();

    // returns camera id to be used
    // as raw input
    int getCameraId();

    // return filename of the image to be used as input
    QString getImageFilename();

    // get qcamera object
    QObject* getQMLCamera();

    // get frequency of updating
    double getUpdateMS();

    // returns resulting MVP matrix
    QMatrix4x4 getMVPMatrix();

    // returns image width
    int getImageWidth();

    // get processed image provider
    QQuickImageProvider* getImageProvider();

public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // initialize from a QCamera object
    void setQMLCamera(QObject *camera);

    // start timer
    void setUpdateMS(double value);

    // call to emit newMVPMatrix signal
    void newMVPMatrixSlot();

    void setImageWidth(int new_width);

    // updates MVP matrix
    void update();

private:
    // width of the input camera image
    int image_width;

    // timer to update MVP matrix
    QTimer timer;

    // QCamera object
    QObject* qml_camera;

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
    MarkerBackEnd marker_backend;

    // ModelViewProjection matrix
    // provider
    MVPProvider* mvp_provider;

    // Inertial Measurement object
    IMU* imu;

    // MVP decorated with last pose from IMU
    IMUMVPDecorator* mvp_imu_decorated;

    // Pose Estimator
    PosePredictor* predictor;

    // Tracking object
    TrackingDecorator* tracking;
signals:
    // notify QML part when new matrix is available
    void newMVPMatrix();

    // notify about new image
    void imageUpdated();
};

#endif // CELLULOAR_H
