#include "threadedqmlar.h"
#include "qmlar.h"
#include "timelogger.h"

ThreadedQMLAR::ThreadedQMLAR()
{
    TimeLoggerLog("%s", "Starting Threaded QMLAR");

    // creating AR object and a thread
    instance = new QMLAR();
    thread = new QThread();

    qRegisterMetaType<Qt::ApplicationState>("Qt::ApplicationState");

    // moving AR to thread
    instance->moveToThread(thread);

    // calls to this object -> another thread
    connect(this, SIGNAL(setCameraIdSignal(int)), instance, SLOT(setCameraId(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageFilenameSignal(QString)), instance, SLOT(setImageFilename(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageWidthSignal(int)), instance, SLOT(setImageWidth(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(startCameraSignal(void)), instance, SLOT(startCamera()), Qt::QueuedConnection);
    connect(this, SIGNAL(setFilterAlphaSignal(double)), instance, SIGNAL(newFilterAlpha(double)));

    // events from AR -> this object
    connect(instance, SIGNAL(imageUpdated(void)), this, SIGNAL(imageUpdated(void)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newBlobs(QVariantList)), this, SIGNAL(newBlobs(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMarkers(QVariantList)), this, SIGNAL(newMarkers(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SIGNAL(newMVPMatrix(QMatrix4x4)), Qt::QueuedConnection);

    // starting AR
    thread->start();
}

int ThreadedQMLAR::getCameraId()
{
    return instance->getCameraId();
}

QString ThreadedQMLAR::getImageFilename()
{
    return instance->getImageFilename();
}

QMatrix4x4 ThreadedQMLAR::getMVPMatrix()
{
    return instance->getMVPMatrix();
}

int ThreadedQMLAR::getImageWidth()
{
    return instance->getImageWidth();
}

QQuickImageProvider *ThreadedQMLAR::getImageProvider()
{
    return instance->getImageProvider();
}

QObject *ThreadedQMLAR::getCamera()
{
    return instance->getCamera();
}

QVariantList ThreadedQMLAR::getBlobs()
{
    return instance->getBlobs();
}

QVariantList ThreadedQMLAR::getMarkers()
{
    return instance->getMarkers();
}

double ThreadedQMLAR::getFPSMean()
{
    return instance->getFPSMean();
}

double ThreadedQMLAR::getFPSStd()
{
    return instance->getFPSStd();
}

bool ThreadedQMLAR::markers_visible()
{
    return instance->getMarkers().size() > 0;
}

double ThreadedQMLAR::getFilterAlpha()
{
    return filter_alpha;
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
