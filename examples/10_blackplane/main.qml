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
    height: 800
    width: 1300

    Button {
        anchors.left: parent.left
        anchors.top: parent.top
        text: "Big"
        onClicked: arComponent.arSceneObject.isBig = true;
    }

    Button {
        anchors.right: parent.right
        anchors.top: parent.top
        text: "Small"
        onClicked: arComponent.arSceneObject.isBig = false;
    }

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // setting width
        width: 1300
    }
}
