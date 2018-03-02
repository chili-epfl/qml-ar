#ifndef CELLULOAR_H
#define CELLULOAR_H

#include <QtCore>
#include <QVector2D>
#include <QtQml>
#include <QString>
#include <QVariantList>
#include <QMatrix4x4>
#include <QQuickImageProvider>

#include "markerstorage.h"

// forward declarations
class ImageProviderAsync;
class UchiyaMarkerDetector;
class PerspectiveCamera;
class MarkerBackEnd;
class MarkerMVPProvider;
class CalibratedCamera;
class IMUMVPDecorator;
class PosePredictor;
class BlackenRest;
class BlobDetector;
class QtCamera2QML;
class ImageScaler;
class MarkerStorage;
class HSVIntervalDetector;
class HueThreshold;
class IMU;
class FPSCalculator;
class PoseFilter;
class LatencyCalculator;

/*
 * This class is the C++/QML interface to the
 * QML AR library
 */

class QMLAR : public QObject
{ Q_OBJECT
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

    // return list of blobs
    QVariantList getBlobs();

    // return list of marker corners
    QVariantList getMarkers();

    // get FPS mean/std
    double getFPSMean();
    double getFPSStd();

    // get Latency mean/std
    double getLatencyMean();
    double getLatencyStd();
public slots:
    // initialize from camera id (default value -1)
    void setCameraId(int camera_id = -1);

    // initialize from image
    void setImageFilename(QString filename);

    // set width of the image to process (on startup)
    void setImageWidth(int new_width);

    // start camera if required
    void startCamera();

    // set MVP for QML
    void setMVP(PipelineContainer<QMatrix4x4> mvp);
    void setMV(PipelineContainer<QMatrix4x4> mv);
    void setP(PipelineContainer<QMatrix4x4> p);

    // set blobs from detector
    void setBlobs(PipelineContainer<QVector<QVector2D>> blobs);

    // set markers
    void setMarkers(PipelineContainer<MarkerStorage> storage);

    // sets mean and std hue of dots
    void hueAvailable(double mean, double std);

    // set image and dots from detector
    void setDots(PipelineContainer<QPair<QImage, QVector<QVector2D>>> image_dots);

    // set last info
    void setInfo(PipelineContainerInfo info);

private:
    // initialization semaphore
    QSemaphore init_sem;

    // vector with blobs
    QVector<QVector2D> last_blobs;

    // storage for markers
    MarkerStorage* marker_storage;

    // info from mvp provider
    PipelineContainerInfo* last_info;

    // mvp buffer for QML
    QMatrix4x4 mvp_buffer;
    QMatrix4x4 mv_buffer, p_buffer;

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
    MarkerBackEnd* marker_backend;

    // ModelViewProjection matrix
    // provider
    MarkerMVPProvider* mvp_provider;

    // Inertial Measurement object
    IMU* imu;

    // MVP decorated with last pose from IMU
    IMUMVPDecorator* mvp_imu_decorated;

    // Pose Estimator
    PosePredictor* predictor;

    // Tracking object
    BlackenRest* blacken_rest;

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

    // detector of dots color interval
    HSVIntervalDetector* hsv_interval;

    // Hue HSV thresholder
    HueThreshold* hue_threshold;

    // fps mean/std values
    FPSCalculator* fps;

    // latency mean/std values
    LatencyCalculator* latency;

    // 3D pose low-pass filter
    PoseFilter* pose_filter;

    // connect underlying objects
    void connectAll();

    // threads for objects
    QVector<QThread*> threads;
signals:
    // notify QML part when new matrix is available
    void newMVPMatrix(QMatrix4x4 mvp);
    void newMVMatrix(QMatrix4x4 mv);
    void newPMatrix(QMatrix4x4 p);

    // notify about new image
    void imageUpdated();

    // on new blobs from detector
    void newBlobs(QVariantList);

    // on new markers
    void newMarkers(QVariantList);

    // set filter alpha
    void newFilterAlpha(double alpha);
};

#endif // CELLULOAR_H
