/**
 * @file main.qml
 * @brief This project renders a Chest and a cuboid using qml-ar
 *
 * The cuboid will be at (0, 0) and the chest at (210, 297)
 * The default camera on Desktop is "1" (second), and front on
 * Android. Note that you need to print the ar_demo_sheet.pdf on A4
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import AR 1.0

/** @brief This project renders a Chest and a cuboid using qml-ar */
Window {
    id: window
    visible: true
    title: qsTr("QML-AR: Chest")

    // not allowing the window to be too small
    minimumWidth: arComponent.width
    minimumHeight: arComponent.height

    /** crearing AR component */
    ARComponent {
        id: arComponent

        /** load scene */
        arSceneComponent: Qt.createComponent("Activity.qml")

        /** add chest coordinates as parameters */
        arSceneParameters: {'chest_x': 210, 'chest_y': 297}

        /** set width to 1500 on tablet (regular mode) */
        width: 500

        /** do show fps? */
        show_fps: true

        /** show blobs? */
        show_blobs: false

        /** show markers? */
        show_markers: false
    }
}
