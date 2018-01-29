#include "threadedqmlar.h"
#include "qmlar.h"
#include "timelogger.h"

ThreadedQMLAR::ThreadedQMLAR()
{
    TimeLoggerLog("%s", "Starting Threaded QMLAR");
    instance = new QMLAR();
    thread = new QThread();

    instance->moveToThread(thread);

    connect(this, SIGNAL(setCameraIdSignal(int)), instance, SLOT(setCameraId(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageFilenameSignal(QString)), instance, SLOT(setImageFilename(QString)), Qt::QueuedConnection);
    connect(this, SIGNAL(setImageWidthSignal(int)), instance, SLOT(setImageWidth(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(startCameraSignal()), instance, SLOT(startCamera()), Qt::QueuedConnection);

    connect(instance, SIGNAL(imageUpdated()), this, SIGNAL(imageUpdated()), Qt::QueuedConnection);
    connect(instance, SIGNAL(newBlobs(QVariantList)), this, SIGNAL(newBlobs(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMarkers(QVariantList)), this, SIGNAL(newMarkers(QVariantList)), Qt::QueuedConnection);
    connect(instance, SIGNAL(newMVPMatrix(QMatrix4x4)), this, SIGNAL(newMVPMatrix(QMatrix4x4)), Qt::QueuedConnection);

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
