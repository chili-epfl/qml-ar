/**
 * @file qtcamera2qml.cpp
 * @brief This is a wrapper over a QCamera which allows using it in QML
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-07-25
 */

#include "qtcamera2qml.h"

QtCamera2QML::QtCamera2QML(QCamera* cam)
{
    camera = cam;
}

QObject* QtCamera2QML::mediaObject()
{
    return camera;
}
