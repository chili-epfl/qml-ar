/**
 * @file main.qml
 * @brief This project shows a big black plane in place of marker sheet to test performance with big objects on Android
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project shows a big black plane in place of marker sheet to test performance with big objects on Android */
Window {
    // some window parameters
    visible: true
    height: 1000
    width: 1500

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // setting width
        width: 1500
    }
}
