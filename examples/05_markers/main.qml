/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it show how to react on markers dissapearance.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it show how to react on markers dissapearance. */
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

        // setting width
        width: 500

        // time after which to discard pose
        Component.onCompleted: {
            arComponent.reset_ms = 5000;
        }
    }

    // showing a huge text when no markers are visible
    Text {
        anchors.centerIn: parent
        font.pixelSize: 30
        text: arComponent.markers_visible ? "" : "NO MARKERS"
        color: "red"
    }
}
