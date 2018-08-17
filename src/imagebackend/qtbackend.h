/**
 * @file qtbackend.h
 * @brief QCamera backend for QML
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef QTBACKEND_H
#define QTBACKEND_H

#include <QElapsedTimer>
#include <QString>
#include <QtConcurrent>
#include <QtMultimedia/QCamera>
#include <QtMultimedia/QVideoProbe>
#include "cameraframegrabber.h"
#include "voidviewfinder.h"
#include "imageproviderasync.h"

/**
 * QCamera backend for QML
 *
 * works on Linux and Android
 */

class QtCameraBackend : public ImageProviderAsync
{ Q_OBJECT

public:
    /**
    * @brief Cam_id indicates the camera index in QCameraInfo::availableCameras()
    */
    QtCameraBackend(int cam_id = 0);

    /**
    * @brief Callback for main thread
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    virtual ~QtCameraBackend();

    /**
    * @brief Convert frame -> image
    */
    void convert(QVideoFrame *frame, QImage *image);

    /**
    * @brief Return underlying camera object
    */
    QCamera* getCamera();

    /**
    * @brief Start camera
    */
    void start();
protected:
    /**
    * @brief Last image id
    */
    int image_id;

    /**
    * @brief Information about current image
    */
    PipelineContainerInfo image_info;

    /**
    * @brief Is frame available?
    */
    bool frame_available = 0;

    /**
     * @brief Last frame from the camera
     */
    QVideoFrame last_frame;

    /**
    * @brief Image buffer
    * Is written in processQImage slot
    * Is read in requestPixmap
    * TODO: thread safety (now can crash randomly)
    */
    QImage buf;

    /**
    * @brief Camera object
    */
    QCamera* camera;

    /**
    * @brief LINUX
    */
    CameraFrameGrabber* frameGrabber;

    /**
    * @brief ANDROID
    * QVideoProbe for Android version
    */
    QVideoProbe* probe;

    /**
    * @brief Void viewfinder for Android version
    */
    VoidViewFinder* viewfinder;

    /**
    * @brief Initialize the object after setting camera ptr
    */
    void init();

    /**
     * @brief Send HSV values to NV21VideoFilter
     * Only for Android API >= 26
     */
    void updateHSVThreshold();

    /**
    * @brief If set to true, will install a void viewfinder
    */
    bool need_viewfinder;

    /**
    * @brief Thread result
    */
    QFutureWatcher<QImage> watcher;

    /**
    * @brief Use output of VideoFilter (gpu-based)?
    * Currently works only for Android API >= 26
    */
    bool use_gpu = false;

    /**
     * @brief HSV Mean/+- Hue (Full scale: 0-360) for GPU thresholding
     * Only for Android API >= 26.
     * Use HueThreshold class (CPU) on other platforms
     */
    double mean_h, delta_h;

    /**
     * @brief HSV Min/MAX SV (Full scale: 0-255) for GPU thresholding
     * Only for Android API >= 26
     * Use HueThreshold class (CPU) on other platforms
     */
    double min_s, max_s, min_v, max_v;

public slots:
    /**
    * @brief Set this image as buffer
    */
    void processQImage(QImage img);

    /**
    * @brief Set this image as buffer and send it to listeners
    */
    void processAndSendQImage(PipelineContainer<QImage> img);

    /**
    * @brief Process frame in thread
    */
    void processQVideoFrame(const QVideoFrame &frame);

    /**
    * @brief Called on thread finish
    */
    void handleFinished();

    /**
    * @brief Set Hue to threshold on (Full scale: 0-360)
    * mean +- std will be kept
    */
    void setColor(double mean, double delta);

    /**
     * @brief Set min/max Value (Full scale: 0-255)
     * @param min_ min value
     * @param max_ max value
     */
    void setVMinMax(double min_, double max_);

    /**
     * @brief Set min/max Saturation (Full scale: 0-255)
     * @param min_ min saturation
     * @param max_ max saturation
     */
    void setSMinMax(double min_, double max_);

    /**
     * @brief Calculated current marker corners, range 0-1
     */
    void setPolygon(QPolygonF marker);

    /**
     * @brief Make this object active/inactive
     * @param active The new active/inactive parameter
     */
    void setActive(bool active);

    /**
     * @brief Enable/disable shader output on top-left corner.
     * Only for Android API 26 and higher
     */
    void setShowOutput(bool show);
};

#endif // QTBACKEND_H
