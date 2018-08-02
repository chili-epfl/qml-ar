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
    * @brief If set to true, will install a void viewfinder
    */
    bool need_viewfinder;

    /**
    * @brief Thread result
    */
    QFutureWatcher<QImage> watcher;

public slots:
    /**
    * @brief Set this image as buffer
    */
    void processQImage(QImage img);

    /**
    * @brief Process frame in thread
    */
    void processQVideoFrame(const QVideoFrame &frame);

    /**
    * @brief Called on thread finish
    */
    void handleFinished();
};

#endif // QTBACKEND_H
