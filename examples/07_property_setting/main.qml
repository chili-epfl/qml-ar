/**
 * @file main.qml
 * @brief This project shows how to manage properties of your the 3D scene
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project shows how to manage properties of your the 3D scene */
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

        // set initial translation
        // (initial parameters for your scene)
        arSceneParameters: {'translation': Qt.vector3d(100, 100, 0)}

        // setting width
        width: 500
    }

    // changing the coordindates to random ones each 3 seconds
    // (can change parameters of your scene at runtime)
    Timer {
        running: true
        repeat: true
        interval: 3000
        onTriggered: {
            // place the cube somewhere on the A4 sheet and somewhere in z
            arComponent.arSceneObject.translation.x = Math.random() * 210;
            arComponent.arSceneObject.translation.y = Math.random() * 297;
            arComponent.arSceneObject.translation.z = (Math.random() - 0.5) * 40;
        }
    }
}
