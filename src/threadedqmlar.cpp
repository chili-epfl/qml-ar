/**
 * @file threadedqmlar.cpp
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "threadedqmlar.h"
#include "qmlar.h"
#include "timelogger.h"

ThreadedQMLAR::ThreadedQMLAR()
{
    TimeLoggerLog("%s", "Starting Threaded QMLAR");

    /**
    * @brief Creating AR object and a thread
    */
    instance = new QMLAR();
    thread = new QThread();

    qRegisterMetaType<Qt::ApplicationState>("Qt::ApplicationState");

    /**
    * @brief Moving AR to thread
    */
    instance->moveToThread(thread);

    /**
    * @brief Calls to this object -> another thread
    */
    connect(this, SIGNAL(setCameraIdSignal(int)), instance, SLOT(setCameraId(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageFilenameSignal(QString)), instance, SLOT(setImageFilename(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageWidthSignal(int)), instance, SLOT(setImageWidth(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(startCameraSignal(void)), instance, SLOT(startCamera()), Qt::QueuedConnection);
    connect(this, SIGNAL(setFilterAlphaSignal(double)), instance, SIGNAL(newFilterAlpha(double)));

    /**
    * @brief Events from AR -> this object
    */
    connect(instance, SIGNAL(imageUpdated(void)), this, SIGNAL(imageUpdated(void)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newBlobs(QVariantList)), this, SLOT(setBlobs(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMarkers(QVariantList)), this, SLOT(setMarkers(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SLOT(setMVPMatrix(QMatrix4x4)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVMatrix(QMatrix4x4)), this, SLOT(setMVMatrix(QMatrix4x4)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newPMatrix(QMatrix4x4)), this, SLOT(setPMatrix(QMatrix4x4)), Qt::QueuedConnection);

    /**
    * @brief Starting AR
    */
    thread->start();
}

int ThreadedQMLAR::getCameraId()
{
    /**
    * @brief Works since is set only once from another thread
    */
    return instance->getCameraId();
}

QString ThreadedQMLAR::getImageFilename()
{
    /**
    * @brief Works since is set only once from another thread
    */
    return instance->getImageFilename();
}

QMatrix4x4 ThreadedQMLAR::getMVPMatrix()
{
    return mvp_matrix;
}

QMatrix4x4 ThreadedQMLAR::getMVMatrix()
{
    return mv_matrix;
}

QMatrix4x4 ThreadedQMLAR::getPMatrix()
{
    return p_matrix;
}

int ThreadedQMLAR::getImageWidth()
{
    /**
    * @brief Works since is set only once from another thread
    */
    return instance->getImageWidth();
}

QQuickImageProvider *ThreadedQMLAR::getImageProvider()
{
    /**
    * @brief Works since is set only once from another thread
    */
    return instance->getImageProvider();
}

QObject *ThreadedQMLAR::getCamera()
{
    /**
    * @brief Works since is set only once from another thread
    */
    return instance->getCamera();
}

QVariantList ThreadedQMLAR::getBlobs()
{
    return last_blobs;
}

QVariantList ThreadedQMLAR::getMarkers()
{
    return last_markers;
}

double ThreadedQMLAR::getFPSMean()
{
    /**
    * @brief Works since mean is a fixed-location variable
    * Which is set from a different thread
    */
    return instance->getFPSMean();
}

double ThreadedQMLAR::getFPSStd()
{
    /**
    * @brief Works since std is a fixed-location variable
    * Which is set from a different thread
    */
    return instance->getFPSStd();
}

double ThreadedQMLAR::getLatencyMean()
{
    /**
    * @brief Works since mean is a fixed-location variable
    * Which is set from a different thread
    */
    return instance->getLatencyMean();
}

double ThreadedQMLAR::getLatencyStd()
{
    /**
    * @brief Works since std is a fixed-location variable
    * Which is set from a different thread
    */
    return instance->getLatencyStd();
}

bool ThreadedQMLAR::markers_visible()
{
    return last_markers.size() > 0;
}

double ThreadedQMLAR::getFilterAlpha()
{
    return filter_alpha;
}

bool ThreadedQMLAR::poseValid()
{
    return !mvp_matrix.isIdentity();
}

double ThreadedQMLAR::getFrameDrop()
{
    return instance->getFrameDrop();
}

int ThreadedQMLAR::getFrameDelay()
{
    return instance->getFrameDelay();
}

void ThreadedQMLAR::setCameraId(int camera_id)
{
    emit setCameraIdSignal(camera_id);
}

void ThreadedQMLAR::setImageFilename(QString filename)
{
    emit setImageFilenameSignal(filename);
}

void ThreadedQMLAR::setImageWidth(int new_width)
{
    emit setImageWidthSignal(new_width);
}

void ThreadedQMLAR::startCamera()
{
    emit startCameraSignal();
}

void ThreadedQMLAR::setFilterAlpha(double alpha)
{
    this->filter_alpha = alpha;
    emit setFilterAlphaSignal(alpha);
}

void ThreadedQMLAR::setBlobs(QVariantList that)
{
    this->last_blobs = that;
    emit newBlobs(last_blobs);
}

void ThreadedQMLAR::setMarkers(QVariantList that)
{
    this->last_markers = that;
    emit newMarkers(last_markers);
}

void ThreadedQMLAR::setMVPMatrix(QMatrix4x4 that)
{
    this->mvp_matrix = that;
    emit newMVPMatrix(mvp_matrix);
}

void ThreadedQMLAR::setMVMatrix(QMatrix4x4 that)
{
    this->mv_matrix = that;
    emit newMVMatrix(mv_matrix);
}

void ThreadedQMLAR::setPMatrix(QMatrix4x4 that)
{
    this->p_matrix = that;
    emit newPMatrix(p_matrix);
}
