/**
 * @file main.qml
 * @brief This project shows how to use camera 0 for AR
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project shows how to use camera 0 for AR */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // set camera 0 to use
        camera_id: 0

        // setting width
        width: 500
    }
}
