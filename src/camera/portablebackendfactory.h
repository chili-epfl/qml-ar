/**
 * @file portablebackendfactory.h
 * @brief 
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#ifndef PORTABLEBACKENDFACTORY_H
#define PORTABLEBACKENDFACTORY_H

#include "imageproviderasync.h"

/*
 * This class returns a camera backend
 * that runs both on Android and Linux
 */

class PortableCameraBackendFactory
{
public:
    /**
    * @brief Returns a camera id which is used
    * By default on this platform
    */
    static int getDefaultCameraId();

    /**
    * @brief -1 is an invalid value telling the method
    * To use the default one (platform-specific)
    */
    static ImageProviderAsync* getBackend(int camera_id = -1);

    /**
    * @brief True if a viewfinder output is available for the camera
    */
    static bool cameraViewfinderAvailable();
};

#endif // PORTABLEBACKENDFACTORY_H
