/**
 * @file qmlar.h
 * @brief This class is the C++/QML interface to the
 * QML AR library
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

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

//Forward declarations
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
class FramesDelayCalculator;

/**
 * @brief This class is the C++/QML interface to the
 * QML AR library
 */

class QMLAR : public QObject
{ Q_OBJECT
public:
    /**
    * @brief Enum for initialization type
    */
    enum InitType
    {
        INIT_CAMERA,
        INIT_IMAGE
    };
    Q_ENUMS(InitType)

    /**
    * @brief Enum for output type
    */
    enum OutputImage
    {
        OUTPUT_CAMERA,
        OUTPUT_MARKERS
    };
    Q_ENUMS(OutputImage)

    /**
    * @brief Initialization type
    */
    int init_type;

    /**
    * @brief Empty constructor
    * Object must be initialized with
    * One of the methods below
    */
    QMLAR();

    /**
    * @brief Returns camera id to be used
    * As raw input
    */
    int getCameraId();

    /**
    * @brief Return filename of the image to be used as input
    */
    QString getImageFilename();

    /**
    * @brief Returns resulting MVP matrix
    */
    QMatrix4x4 getMVPMatrix();

    /**
    * @brief Returns image width
    */
    int getImageWidth();

    /**
    * @brief Get processed image provider
    */
    QQuickImageProvider* getImageProvider();

    /**
    * @brief Return camera object
    */
    QObject *getCamera();

    /**
    * @brief Return list of blobs
    */
    QVariantList getBlobs();

    /**
    * @brief Return list of marker corners
    */
    QVariantList getMarkers();

    /** @brief Get mean FPS */
    double getFPSMean();

    /** @brief Get standard deviation of FPS */
    double getFPSStd();

    /** @brief Get mean latency (ms) */
    double getLatencyMean();

    /** @brief Get std. of latency */
    double getLatencyStd();

    /**
    * @brief Returns framedrop fraction
    */
    double getFrameDrop();

    /**
    * @brief Delay in frames to compensate for latency
    */
    int getFrameDelay();
public slots:
    /**
    * @brief Initialize from camera id (default value -1)
    */
    void setCameraId(int camera_id = -1);

    /**
    * @brief Initialize from image
    */
    void setImageFilename(QString filename);

    /**
    * @brief Set width of the image to process (on startup)
    */
    void setImageWidth(int new_width);

    /**
    * @brief Start camera if required
    */
    void startCamera();

    /**
    * @brief Set MVP for QML
    */
    void setMVP(PipelineContainer<QMatrix4x4> mvp);

    /**
    * @brief Set MV for QML
    */
    void setMV(PipelineContainer<QMatrix4x4> mv);

    /**
    * @brief Set P for QML
    */
    void setP(PipelineContainer<QMatrix4x4> p);

    /**
    * @brief Set blobs from detector
    */
    void setBlobs(PipelineContainer<QVector<QVector2D>> blobs);

    /**
    * @brief Set markers
    */
    void setMarkers(PipelineContainer<MarkerStorage> storage);

    /**
    * @brief Sets mean and std hue of dots
    */
    void hueAvailable(double mean, double std);

    /**
    * @brief Set image and dots from detector
    */
    void setDots(PipelineContainer<QPair<QImage, QVector<QVector2D>>> image_dots);

    /**
    * @brief Set last info
    */
    void setInfo(PipelineContainerInfo info);

    /**
    * @brief Set running/paused
    */
    void setRunning(bool running);

private:
    /**
    * @brief Initialization semaphore
    */
    QSemaphore init_sem;

    /**
    * @brief Vector with blobs
    */
    QVector<QVector2D> last_blobs;

    /**
    * @brief Storage for markers
    */
    MarkerStorage* marker_storage;

    /**
    * @brief Info from mvp provider
    */
    PipelineContainerInfo* last_info;

    /**
    * @brief Mvp buffer for QML
    */
    QMatrix4x4 mvp_buffer;
    QMatrix4x4 mv_buffer, p_buffer;

    /**
    * @brief Width of the input camera image
    */
    int image_width;

    /**
    * @brief Name of the image to be used
    * As raw input
    */
    QString image_filename;

    /**
    * @brief Camera id to be used
    * As raw input
    */
    int camera_id;

    /**
    * @brief True iff can obtain MVP matrix
    */
    bool is_initialized;

    /**
    * @brief Instantiate objects
    */
    void initialize();

    /**
    * @brief Image provider (camera)
    */
    ImageProviderAsync* raw_provider;

    /**
    * @brief Marker detector
    */
    UchiyaMarkerDetector* detector;

    /**
    * @brief Camera matrix
    */
    CalibratedCamera* camera_matrix;

    /**
    * @brief Perspective camera model
    */
    PerspectiveCamera* perspective_camera;

    /**
    * @brief Marker backend for visualizing
    * Detected markers
    */
    MarkerBackEnd* marker_backend;

    /**
    * @brief ModelViewProjection matrix
    * Provider
    */
    MarkerMVPProvider* mvp_provider;

    /**
    * @brief Inertial Measurement object
    */
    IMU* imu;

    /**
    * @brief MVP decorated with last pose from IMU
    */
    IMUMVPDecorator* mvp_imu_decorated;

    /**
    * @brief Pose Estimator
    */
    PosePredictor* predictor;

    /**
    * @brief Tracking object
    */
    BlackenRest* blacken_rest;

    /**
    * @brief Blob detector
    */
    BlobDetector* blob_detector;

    /**
    * @brief Drawn blobs
    */
    QImage detected_blobs;

    /**
    * @brief Maximal number of dots to detect
    */
    const int max_dots = 50;

    /**
    * @brief Camera wrapper
    */
    QtCamera2QML* camera_wrapper;

    /**
    * @brief Scaler for input images
    */
    ImageScaler* scaler;

    /**
    * @brief Detector of dots color interval
    */
    HSVIntervalDetector* hsv_interval;

    /**
    * @brief Hue HSV thresholder
    */
    HueThreshold* hue_threshold;

    // fps mean/std values
    FPSCalculator* fps;

    // latency mean/std values
    LatencyCalculator* latency;

    /**
    * @brief 3D pose low-pass filter
    */
    PoseFilter* pose_filter;

    /**
    * @brief Delay in frames calculator
    */
    FramesDelayCalculator* delay;

    /**
    * @brief Connect underlying objects
    */
    void connectAll();

    /**
    * @brief Threads for objects
    */
    QVector<QThread*> threads;

    /**
    * @brief Number of frames received
    */
    int n_frames_received;

    /**
    * @brief Fraction of frames dropped
    */
    double framedrop_fraction;

    /**
     * @brief is QML running or paused?
     */
    bool is_running;

    /**
     * @brief HSV Mean/+- Hue (Full scale: 0-360)
     */
    double mean_h, delta_h;

    /**
     * @brief HSV Min/MAX SV (Full scale: 0-255)
     */
    double min_s, max_s, min_v, max_v;
signals:
    /**
    * @brief Notify QML part when new matrix is available
    */
    void newMVPMatrix(QMatrix4x4 mvp);

    /**
    * @brief Notify QML part when new matrix is available
    */
    void newMVMatrix(QMatrix4x4 mv);

    /**
    * @brief Notify QML part when new matrix is available
    */
    void newPMatrix(QMatrix4x4 p);

    /**
    * @brief Notify about new image
    */
    void imageUpdated();

    /**
    * @brief On new blobs from detector
    */
    void newBlobs(QVariantList);

    /**
    * @brief On new markers
    */
    void newMarkers(QVariantList);

    /**
    * @brief Set filter alpha
    */
    void newFilterAlpha(double alpha);
};

#endif // CELLULOAR_H
