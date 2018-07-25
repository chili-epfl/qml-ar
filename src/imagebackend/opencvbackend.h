/**
 * @file opencvbackend.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef CV_BACKEND_H
#define CV_BACKEND_H

#include <QtConcurrent>
#include <QQuickImageProvider>
#include <QString>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "imageproviderasync.h"

/*
 * OpenCV camera backend for QML
 * Uses cv::VideoCapture
 */

class OpenCVCameraBackend : public ImageProviderAsync
{ Q_OBJECT
public:
    /**
    * @brief Empty constructor
    */
    OpenCVCameraBackend();

    virtual ~OpenCVCameraBackend();

    /**
    * @brief Copy constructor
    */
    OpenCVCameraBackend(const OpenCVCameraBackend &backend);

    /**
    * @brief Initialize with camera_id
    */
    OpenCVCameraBackend(int cam_id = 0);

    /**
    * @brief Get image from the camera
    */
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    /**
    * @brief Request new image from camera
    */
    void request();

    /**
    * @brief Obtain image from OpenCV
    */
    QImage getImage();
private:
    /**
    * @brief Open the camera
    */
    void setupCV();

    /**
    * @brief Id of the camera in VideoCapture(...)
    */
    int camera_id;

    /**
    * @brief Camera opened?
    */
    bool is_initialized = false;

    /**
    * @brief Buffer for the image
    */
    QImage buf;

    /**
    * @brief CV VideoCapture
    */
    cv::VideoCapture* stream = NULL;

    /**
    * @brief Thread worker
    */
    QFutureWatcher<QImage> watcher;

    /**
    * @brief Last image identity
    */
    int image_id;

public slots:
    /**
    * @brief Get result from future
    */
    void handleFinished();
};

#endif // CV_BACKEND_H
