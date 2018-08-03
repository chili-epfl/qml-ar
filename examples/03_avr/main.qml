/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * The Augmented Virtual Reality (or Mixed Reality) is enabled.
 *
 * The default camera on Desktop is "1" (second), and front on
 * Android. Note that you need to print the ar_demo_sheet.pdf on A4
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import AR 1.0

/** @brief This project renders a cube at (0, 0, 0) using qml-ar in AVR */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500

    // crearing AR component
    ARComponent {
        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // setting width
        width: 500

        // enable AVR mode
        avr_mode: true

        // force image on Android
        force_image: true
    }
}
