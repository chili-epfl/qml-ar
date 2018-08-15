/**
 * @file main.qml
 * @brief This project shows how to pause/restart QMLAR
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project shows how to pause/restart QMLAR */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500

    Button {
        anchors.left: parent.left
        id: button_p
        text: "Pause"
        onClicked: arComponent.is_running = false
    }

    Button {
        anchors.right: parent.right
        id: button_s
        text: "Start"
        onClicked: arComponent.is_running = true
    }

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // setting width
        width: 500
    }
}
