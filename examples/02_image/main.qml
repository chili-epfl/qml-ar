/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Image input is used instead of a camera.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import AR 1.0

/** This project renders a cube at (0, 0, 0) using qml-ar.
 * Image input is used instead of a camera. */
Window {
    // some window parameters
    id: root
    visible: true
    height: 500
    width: 500

    // crearing AR component
    ARComponent {
        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // use image
        init_type: AR.INIT_IMAGE

        // setting width
        width: 500
    }
}
