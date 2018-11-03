/**
 * @file threadedqmlar.h
 * @brief This class is a wrapper around QMLAR object
 * which is placed at a separate thread and controlled
 * by this object
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef THREADEDQMLAR_H
#define THREADEDQMLAR_H

#include <QObject>
#include <QThread>
#include <QQuickImageProvider>
#include "qmlar.h"

/**
 * @brief This class is a wrapper around QMLAR object
 * which is placed at a separate thread and controlled
 * by this object
 */

class ThreadedQMLAR : public QObject
{ Q_OBJECT
    /** @brief Number of the camera to use. If set, using camera input */
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)

    /** @brief Path to the image to use. If set, will use image input */
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)

    /** @brief QCamera mediaobject for Viewfinder (if available) */
    Q_PROPERTY(QObject* camera READ getCamera)

    /** @brief OpenGL ModelViewProjection matrix from the pipeline */
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)

    /** @brief OpenGL ModelView matrix from the pipeline */
    Q_PROPERTY(QMatrix4x4 mv_matrix READ getMVMatrix NOTIFY newMVPMatrix)

    /** @brief OpenGL Projection matrix from the pipeline */
    Q_PROPERTY(QMatrix4x4 p_matrix READ getPMatrix NOTIFY newMVPMatrix)

    /** @brief The width of the image to be used inside the pipeline */
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)

    /** @brief The list of blobs on detected markers (QVector2D as element) */
    Q_PROPERTY(QVariantList blobs READ getBlobs NOTIFY newBlobs)

    /** @brief The list of detected markers, each containing an array of marker corners */
    Q_PROPERTY(QVariantList markers READ getMarkers NOTIFY newMarkers)

    /** @brief The mean FPS value at this moment */
    Q_PROPERTY(double FPSMean READ getFPSMean NOTIFY imageUpdated)

    /** @brief The std FPS value at this moment */
    Q_PROPERTY(double FPSStd READ getFPSStd NOTIFY imageUpdated)

    /** @brief The mean Latency value at this moment */
    Q_PROPERTY(double LatencyMean READ getLatencyMean NOTIFY imageUpdated)

    /** @brief The std Latency value at this moment */
    Q_PROPERTY(double LatencyStd READ getLatencyStd NOTIFY imageUpdated)

    /** @brief Are markers visible right now? */
    Q_PROPERTY(bool markers_visible READ markers_visible NOTIFY newMarkers)

    /** @brief Is the pose (MVP/MV matrix) valid at this moment? */
    Q_PROPERTY(bool pose_valid READ poseValid NOTIFY newMVPMatrix)

    /** @brief Filtering coefficient for the pose */
    Q_PROPERTY(double filter_alpha READ getFilterAlpha WRITE setFilterAlpha)

    /** @brief The current percentage of dropped frames at this moment */
    Q_PROPERTY(double framedrop READ getFrameDrop NOTIFY imageUpdated)

    /** @brief Current delay in frames from capture to computed pose */
    Q_PROPERTY(int frame_delay READ getFrameDelay NOTIFY imageUpdated)

    /** @brief Pause/stop QMLAR */
    Q_PROPERTY(int running READ getRunning WRITE setRunning)

    /** @brief Show shader output on top-left of the input frame? Android API >= 26 only. */
    Q_PROPERTY(bool show_shader_output READ getShowShader WRITE setShowShader)

    // HSV Threshold

    /** @brief Mean Hue (HSV Threshold), degrees 0-360 */
    Q_PROPERTY(double hsv_mean_h READ getMeanH WRITE setMeanH NOTIFY never)

    /** @brief Delta Hue (HSV Threshold), degrees 0-360 */
    Q_PROPERTY(double hsv_delta_h READ getDeltaH WRITE setDeltaH NOTIFY never)

    /** @brief Min S (HSV Threshold), 0-255 */
    Q_PROPERTY(double hsv_min_s READ getMinS WRITE setMinS NOTIFY never)

    /** @brief Max S (HSV Threshold), 0-255 */
    Q_PROPERTY(double hsv_max_s READ getMaxS WRITE setMaxS NOTIFY never)

    /** @brief Min V (HSV Threshold), 0-255 */
    Q_PROPERTY(double hsv_min_v READ getMinV WRITE setMinV NOTIFY never)

    /** @brief Max V (HSV Threshold), 0-255 */
    Q_PROPERTY(double hsv_max_v READ getMaxV WRITE setMaxV NOTIFY never)

    /** @brief Time in ms after which pose is discarded if no markers are present */
    Q_PROPERTY(unsigned reset_ms READ getResetMs WRITE setResetMs NOTIFY never)

private:
    /**
    * @brief Instance to QMLAR object
    */
    QMLAR* instance;

    /**
    * @brief Thread to run in
    */
    QThread* thread;

    /**
    * @brief Cached alpha filter
    */
    double filter_alpha;

    /**
    * @brief Buffers for blobs and markers
    */
    QVariantList last_blobs, last_markers;

    /**
    * @brief MVP matrix buffer
    */
    QMatrix4x4 mvp_matrix, mv_matrix, p_matrix;

    /**
     * @brief Is QMLAR currently running?
     */
    bool is_running;
public:
    /** @brief Empty constructor */
    ThreadedQMLAR();

    /**
    * @brief Get camera ID
    */
    int getCameraId();

    /** @brief Get path to the image */
    QString getImageFilename();

    /** @brief Get ModelViewProjection matrix */
    QMatrix4x4 getMVPMatrix();

    /** @brief Get ModelView matrix */
    QMatrix4x4 getMVMatrix();

    /** @brief Get projection matrix */
    QMatrix4x4 getPMatrix();

    /** @brief Get width of the image in the pipeline */
    int getImageWidth();

    /** @brief Get current image provider */
    QQuickImageProvider *getImageProvider();

    /** @brief Get QCamera media object, if available */
    QObject* getCamera();

    /** @brief Return list of blobs on markers */
    QVariantList getBlobs();

    /** @brief Return list of detected markers */
    QVariantList getMarkers();

    /** @brief FPS: mean */
    double getFPSMean();

    /** @brief FPS: std */
    double getFPSStd();

    /** @brief Latency (ms): mean */
    double getLatencyMean();

    /** @brief Latency (ms): std */
    double getLatencyStd();

    /**
    * @brief At least one marker present
    */
    bool markers_visible();

    /**
    * @brief Filtering coefficient
    */
    double getFilterAlpha();

    /**
    * @brief Is pose valid?
    */
    bool poseValid();

    /**
    * @brief Framedrop fraction
    */
    double getFrameDrop();

    /**
    * @brief Returns delay in frames
    */
    int getFrameDelay();

    /**
    * @brief Is QMLAR currently active?
    */
    bool getRunning();

    /**
     * @brief Show shader output (Android API >= 26)
     * @return dummy value
     */
    bool getShowShader() {return false;}

    /**
     * @brief Mean Hue (HSV Threshold), degrees 0-360
     */
    double getMeanH();

    /**
     * @brief Delta Hue (HSV Threshold), degrees 0-360
     */
    double getDeltaH();

    /**
     * @brief Min S (HSV Threshold), 0-255
     */
    double getMinS();

    /**
     * @brief Max S (HSV Threshold), 0-255
     */
    double getMaxS();

    /**
     * @brief Min V (HSV Threshold), 0-255
     */
    double getMinV();

    /**
     * @brief Max V (HSV Threshold), 0-255
     */
    double getMaxV();

    /** @brief Time w/o markers after which pose is discarded
     * And the scene is hidden */
    unsigned getResetMs();

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
    * @brief Set filtering alpha
    */
    void setFilterAlpha(double alpha);

    /**
    * @brief Set blobs (internal)
    */
    void setBlobs(QVariantList that);

    /**
    * @brief Set markers (internal)
    */
    void setMarkers(QVariantList that);

    /**
    * @brief Set MVP matrix (internal)
    */
    void setMVPMatrix(QMatrix4x4);

    /**
    * @brief Set MV matrix (internal)
    */
    void setMVMatrix(QMatrix4x4);

    /**
    * @brief Set P matrix (internal)
    */
    void setPMatrix(QMatrix4x4);

    /**
    * @brief Set running/paused
    */
    void setRunning(bool);

    /**
     * @brief Show shader output on top-left of input frame?
     * Only for Android API >= 26
     * @param value Show or not
     */
    void setShowShader(bool value);

    /**
     * @brief Mean Hue (HSV Threshold), degrees 0-360
     * @param value Value of Hue in degrees (0-360)
     */
    void setMeanH(double value);

    /**
     * @brief Delta Hue (HSV Threshold), degrees 0-360
     * @param value Value of Delta Hue in degrees (0-360)
     */
    void setDeltaH(double value);

    /**
     * @brief Min S (HSV Threshold), 0-255
     * @param value Min value of S
     */
    void setMinS(double value);

    /**
     * @brief Max S (HSV Threshold), 0-255
     * @param value Max value of S
     */
    void setMaxS(double value);

    /**
     * @brief Min V (HSV Threshold), 0-255
     * @param value Min value of V
     */
    void setMinV(double value);

    /**
     * @brief Max V (HSV Threshold), 0-255
     * @param value Max value of V
     */
    void setMaxV(double value);

    /** @brief Time w/o markers after which pose is discarded
     * And the scene is hidden */
    void setResetMs(unsigned reset_ms);

signals:
    /**
    * @brief Notify QML part when new matrix is available
    */
    void newMVPMatrix(QMatrix4x4);

    /**
    * @brief Notify QML part when new matrix is available
    */
    void newMVMatrix(QMatrix4x4);

    /**
    * @brief Notify QML part when new matrix is available
    */
    void newPMatrix(QMatrix4x4);

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
    * @brief Initialize from camera id (default value -1)
    */
    void setCameraIdSignal(int camera_id = -1);

    /**
    * @brief Initialize from image
    */
    void setImageFilenameSignal(QString filename);

    /**
    * @brief Set width of the image to process (on startup)
    */
    void setImageWidthSignal(int new_width);

    /**
    * @brief Start camera if required
    */
    void startCameraSignal();

    /**
    * @brief Set filtering alpha
    */
    void setFilterAlphaSignal(double alpha);

    /**
     * @brief Never called
     */
    void never();

    /** @brief Called on setResetMs call */
    void setResetMsSignal(unsigned);
};

#endif // THREADEDQMLAR_H
