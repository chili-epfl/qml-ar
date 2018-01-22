#ifndef CELLULOAR_H
#define CELLULOAR_H

#include <QtCore>
#include <QtQml>
#include <QString>
#include "imageproviderasync.h"
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
#include "blobdetector.h"
#include "qtcamera2qml.h"
#include "imagescaler.h"

/*
 * This class is the C++/QML interface to the
 * QML AR library
 */

class QMLAR : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QObject* camera READ getCamera NOTIFY never)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)

public:
    // enum for initialization type
    enum InitType
    {
        INIT_CAMERA,
        INIT_IMAGE
    };
    Q_ENUMS(InitType)

    // enum for output type
    enum OutputImage
    {
        OUTPUT_CAMERA,
        OUTPUT_MARKERS
    };
    Q_ENUMS(OutputImage)

    // initialization type
    int init_type;

    // empty constructor
    // object must be initialized with
    // one of the methods below
    QMLAR();

    // returns camera id to be used
    // as raw input
    int getCameraId();

    // return filename of the image to be used as input
    QString getImageFilename();

    // returns resulting MVP matrix
    QMatrix4x4 getMVPMatrix();

    // returns image width
    int getImageWidth();

    // get processed image provider
    QQuickImageProvider* getImageProvider();

    // return camera object
    QObject *getCamera();

    void connectAll();
public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // call to emit newMVPMatrix signal
    void newMVPMatrixSlot();

    void setImageWidth(int new_width);

    // start camera if required
    void startCamera();

private:
    // width of the input camera image
    int image_width;

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
    ImageProviderAsync* raw_provider;

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

    // blob detector
    BlobDetector* blob_detector;

    // drawn blobs
    QImage detected_blobs;

    // maximal number of dots to detect
    const int max_dots = 50;

    // camera wrapper
    QtCamera2QML* camera_wrapper;

    // scaler for input images
    ImageScaler* scaler;
signals:
    // notify QML part when new matrix is available
    void newMVPMatrix();

    // notify about new image
    void imageUpdated();

    // never called
    void never();
};

#endif // CELLULOAR_H
