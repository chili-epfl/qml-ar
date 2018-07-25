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
    Q_PROPERTY(int camera_id WRITE setCameraId READ getCameraId)
    Q_PROPERTY(QString image_filename WRITE setImageFilename READ getImageFilename)
    Q_PROPERTY(QObject* camera READ getCamera)
    Q_PROPERTY(QMatrix4x4 mvp_matrix READ getMVPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(QMatrix4x4 mv_matrix READ getMVMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(QMatrix4x4 p_matrix READ getPMatrix NOTIFY newMVPMatrix)
    Q_PROPERTY(int image_width READ getImageWidth WRITE setImageWidth)
    Q_PROPERTY(QVariantList blobs READ getBlobs NOTIFY newBlobs)
    Q_PROPERTY(QVariantList markers READ getMarkers NOTIFY newMarkers)
    Q_PROPERTY(double FPSMean READ getFPSMean NOTIFY imageUpdated)
    Q_PROPERTY(double FPSStd READ getFPSStd NOTIFY imageUpdated)
    Q_PROPERTY(double LatencyMean READ getLatencyMean NOTIFY imageUpdated)
    Q_PROPERTY(double LatencyStd READ getLatencyStd NOTIFY imageUpdated)
    Q_PROPERTY(bool markers_visible READ markers_visible NOTIFY newMarkers)
    Q_PROPERTY(bool pose_valid READ poseValid NOTIFY newMVPMatrix)
    Q_PROPERTY(double filter_alpha READ getFilterAlpha WRITE setFilterAlpha)
    Q_PROPERTY(double framedrop READ getFrameDrop NOTIFY imageUpdated)
    Q_PROPERTY(int frame_delay READ getFrameDelay NOTIFY imageUpdated)

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
public:
    ThreadedQMLAR();

    /**
    * @brief Getters
    */
    int getCameraId();
    QString getImageFilename();
    QMatrix4x4 getMVPMatrix();
    QMatrix4x4 getMVMatrix();
    QMatrix4x4 getPMatrix();
    int getImageWidth();
    QQuickImageProvider *getImageProvider();
    QObject* getCamera();
    QVariantList getBlobs();
    QVariantList getMarkers();

    /**
    * @brief FPS
    */
    double getFPSMean();
    double getFPSStd();

    /**
    * @brief Latency
    */
    double getLatencyMean();
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
    * @brief Setters for AR->QML communication
    */
    void setBlobs(QVariantList that);
    void setMarkers(QVariantList that);
    void setMVPMatrix(QMatrix4x4);
    void setMVMatrix(QMatrix4x4);
    void setPMatrix(QMatrix4x4);

signals:
    /**
    * @brief Notify QML part when new matrix is available
    */
    void newMVPMatrix(QMatrix4x4);
    void newMVMatrix(QMatrix4x4);
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
};

#endif // THREADEDQMLAR_H
