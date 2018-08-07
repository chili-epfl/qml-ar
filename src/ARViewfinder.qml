/**
 * @file ARViewfinder.qml
 * @brief This component implements camera image display on Android using OpenGL textures (fast!)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-02
 */

import QtQuick 2.0
import QtMultimedia 5.4
import ch.epfl.chili.qmlar 1.0

/**
 * @brief This component implements camera image display on Android using OpenGL textures (fast!)
 */
VideoOutput {
    id: viewfinder
    visible: true
    anchors.fill: parent
    filters: [nv21filter, androidFilter]//, delayFilter]
    //autoOrientation: true

    // AndroidFilter -- maps+unmaps the data (no change), see the C++ class for explanation
    AndroidFilter {
        id: androidFilter
    }

    // DelayFilter -- delays the image by the AR pipeline delay so that 3D objects and image are in sync
    DelayFilter {
        id: delayFilter
    }

    NV21VideoFilter {
        id: nv21filter
    }
}
