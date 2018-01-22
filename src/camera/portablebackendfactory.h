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
    // returns a camera id which is used
    // by default on this platform
    static int getDefaultCameraId();

    // -1 is an invalid value telling the method
    // to use the default one (platform-specific)
    static ImageProviderAsync* getBackend(int camera_id = -1);

    // true if a viewfinder output is available for the camera
    static bool cameraViewfinderAvailable();
};

#endif // PORTABLEBACKENDFACTORY_H
