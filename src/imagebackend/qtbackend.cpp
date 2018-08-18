/**
 * @file qtbackend.cpp
 * @brief QCamera backend for QML
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "qtbackend.h"
#include "qvideoframehelpers.h"
#include <QtMultimedia/QCameraInfo>
#include <QVideoProbe>
#include <QException>
#include "qvideoframehelpers.h"
#include "voidviewfinder.h"

#if __ANDROID_API__ >= 16
#include "nv21videofilter.h"
#include "nv21videofilterrunnable.h"
#endif

QtCameraBackend::QtCameraBackend(int cam_id) : ImageProviderAsync()
{
    // initializing camera
    TimeLoggerLog("Number of cameras: %d", QCameraInfo::availableCameras().size());
    camera = new QCamera(QCameraInfo::availableCameras().at(cam_id));
    camera->setParent(this);
    watcher.setParent(this);
    need_viewfinder = 1;
    frame_available = 0;
    image_id = 0;
    use_gpu = true;
    init();
}

void QtCameraBackend::init()
{
    // initial buffer value is an empty image
    buf = QVideoFrameHelpers::empty();

    // installing camera callback
    // has different implementations for Android/Linux
    // since Android camera is poorly supported in Android NDK (Nov'17)

    // android callback will be installed in start()
#if defined Q_OS_LINUX && !defined Q_OS_ANDROID && !defined QT_BACKEND_FORCE_VIDEOPROBE
    // using custom class as a viewfinder
    frameGrabber = new CameraFrameGrabber();
    camera->setViewfinder(frameGrabber);

    // installing callback
    connect(frameGrabber, SIGNAL(frameAvailable(QImage)), this, SLOT(processQImage(QImage)));
#endif

    // waiting for async output
    connect(&watcher, SIGNAL(finished()), this, SLOT(handleFinished()));
}

void QtCameraBackend::updateHSVThreshold()
{
#if __ANDROID_API__ >= 16
    NV21VideoFilterRunnable* runnable = NV21VideoFilter::runnable;
    if(runnable && use_gpu) {
        runnable->mean_h = mean_h / 360.;
        runnable->delta_h = delta_h / 360.;
        runnable->min_v = min_v / 255.;
        runnable->max_v = max_v / 255.;
        runnable->min_s = min_s / 255.;
        runnable->max_s = max_s / 255.;
    }
#endif
}

void QtCameraBackend::start()
{
#if defined Q_OS_ANDROID || defined QT_BACKEND_FORCE_VIDEOPROBE
    // setting up a viewfinder which does nothing
    //    if(need_viewfinder)
    //    {
    //        viewfinder = new VoidViewFinder;
    //        camera->setViewfinder(viewfinder);
    //    }

    // this object does image grabbing
    probe = new QVideoProbe(this);

    // installing callback
    if(probe->setSource((QMediaObject *) camera))
        connect(probe, SIGNAL(videoFrameProbed(const QVideoFrame &)), this, SLOT(processQVideoFrame(const QVideoFrame &)));
    else
        qFatal("Can't connect probe to camera");
#endif

    // starting camera thread
    camera->start();
}

QImage QtCameraBackend::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{ Q_UNUSED(id) Q_UNUSED(size) Q_UNUSED(requestedSize)
            return buf;
}

QtCameraBackend::~QtCameraBackend() {
}

void QtCameraBackend::processAndSendQImage(PipelineContainer<QImage> img)
{
    buf = img.o();

    // sending image
    emit imageAvailable(PipelineContainer<QImage>
                        (img.o(), img.info().checkpointed("Camera")));

    TimeLoggerThroughput("%s", "[ANALYZE] Begin QtCamera");
}

void QtCameraBackend::processQImage(QImage img)
{
    buf = img;
}

QCamera *QtCameraBackend::getCamera()
{
    return camera;
}

void QtCameraBackend::handleFinished()
{
    // saving to buffer
    processQImage(watcher.result());

    // sending image
    emit imageAvailable(PipelineContainer<QImage>
                        (buf.copy(), image_info.checkpointed("Camera")));

    TimeLoggerThroughput("%s", "[ANALYZE] Begin QtCamera");
}

void QtCameraBackend::setColor(double mean, double delta)
{
    mean_h = mean;
    delta_h = delta;
    updateHSVThreshold();
}

void QtCameraBackend::setVMinMax(double min_, double max_)
{
    min_v = min_;
    max_v = max_;
    updateHSVThreshold();
}

void QtCameraBackend::setSMinMax(double min_, double max_)
{
    min_s = min_;
    max_s = max_;
    updateHSVThreshold();
}

void QtCameraBackend::setPolygon(QPolygonF marker)
{ Q_UNUSED(marker)
#if __ANDROID_API__ >= 16
    NV21VideoFilterRunnable* runnable = NV21VideoFilter::runnable;
    if(runnable && use_gpu) {
        runnable->marker = marker;
    }
#endif
}

void QtCameraBackend::setActive(bool active)
{
    is_active = active;
#if __ANDROID_API__ >= 16
    NV21VideoFilterRunnable* runnable = NV21VideoFilter::runnable;
    if(runnable && use_gpu) {
        runnable->active = active;
    }
#endif
}

void QtCameraBackend::setShowOutput(bool show)
{ Q_UNUSED(show)
#if __ANDROID_API__ >= 16
    NV21VideoFilterRunnable* runnable = NV21VideoFilter::runnable;
    if(runnable && use_gpu) {
        runnable->show_processed = show;
    }
#endif
}

void QtCameraBackend::processQVideoFrame(const QVideoFrame &frame)
{
    // doing nothing if deactivated
    if(!is_active) return;

    TimeLoggerThroughput("%s", "Received image from camera");

    // fast GPU-based processing on Android 16 and higher
#if __ANDROID_API__ >= 16
    // at some point viewfinder and filters should be ready, so it's safe to dereference pointer to the videofilter
    NV21VideoFilterRunnable* runnable = NV21VideoFilter::runnable;
    if(runnable && use_gpu) {
        // switching to videofilter output and disabling videoprobe
        probe->setSource((QMediaObject*) nullptr);
        disconnect(probe, SIGNAL(videoFrameProbed(const QVideoFrame &)), this, SLOT(processQVideoFrame(const QVideoFrame &)));
        updateHSVThreshold();
        connect(runnable, &NV21VideoFilterRunnable::imageConverted, this, &QtCameraBackend::processAndSendQImage, Qt::QueuedConnection);
        TimeLoggerLog("%s", "Switched to VideoFilterInput");
        return;
    }
#endif

    // not converting frame if thread is busy
    if(!watcher.isRunning())
    {
        image_info = PipelineContainerInfo(image_id);
        image_id++;
        image_info.checkpoint("Grabbed");
        QFuture<QImage> future = QtConcurrent::run(&QVideoFrameHelpers::VideoFrameToImage, frame);
        watcher.setFuture(future);
    }
}
