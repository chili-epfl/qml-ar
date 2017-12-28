#include "portablebackendfactory.h"
#include "qtbackend.h"
#include "opencvbackend.h"
#include "timelogger.h"
#include <QtCore>

int PortableCameraBackendFactory::getDefaultCameraId()
{
    int res = -1;
#ifdef Q_OS_ANDROID
    res = 0;
#elif defined Q_OS_LINUX
    res = 1;
#else
    #error "OS other than Android and Linux are not supported"
#endif
    return(res);
}

QQuickImageProvider *PortableCameraBackendFactory::getBackend(int camera_id)
{
    // checking for default value
    if(camera_id == -1)
        camera_id = getDefaultCameraId();

    // creating provider connected to the actual camera
    // using OpenCV for Linux
    // And Qt for Android
    QQuickImageProvider* provider = NULL;
#ifdef Q_OS_ANDROID
    provider = new QtCameraBackend(camera_id);
    TimeLoggerLog("%s", "Using Android camera backend");
#elif defined Q_OS_LINUX
    // using 1 as camera id
    provider = new OpenCVCameraBackend(camera_id);
    TimeLoggerLog("%s", "Using Linux camera backend");
#else
    #error "OS other than Android and Linux are not supported"
#endif

    return provider;
}
