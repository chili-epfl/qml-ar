/**
 * @file portablebackendfactory.cpp
 * @brief This class returns a camera backend
 * that runs both on Android and Linux
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "timelogger.h"
#include "portablebackendfactory.h"
#include "qtbackend.h"
#include "opencvbackend.h"
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

ImageProviderAsync *PortableCameraBackendFactory::getBackend(int camera_id)
{
    // checking for default value
    if(camera_id == -1)
        camera_id = getDefaultCameraId();

    // creating provider connected to the actual camera
    // using OpenCV for Linux
    // And Qt for Android
    ImageProviderAsync* provider = NULL;
#if defined Q_OS_ANDROID || defined QT_BACKEND_FORCE_ON_LINUX
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

bool PortableCameraBackendFactory::cameraViewfinderAvailable()
{
    #if defined Q_OS_ANDROID || defined QT_BACKEND_FORCE_ON_LINUX
        return true;
    #else
        return false;
    #endif
}
