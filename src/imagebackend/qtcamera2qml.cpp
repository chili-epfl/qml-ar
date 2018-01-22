#include "qtcamera2qml.h"

QtCamera2QML::QtCamera2QML(QCamera* cam)
{
    camera = cam;
}

QObject* QtCamera2QML::mediaObject()
{
    return camera;
}
