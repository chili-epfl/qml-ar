/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it allows to estimate the clicked point x, y in the world.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import AR 1.0
import QtQuick 2.6
import QtQuick.Controls 2.3


/** @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it allows to estimate the clicked point x, y in the world.*/
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

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            popup_text.text = "Clicked at x = " + Math.round(x_mm, 1) + "mm y = " + Math.round(y_mm, 1) + "mm z = 0mm.\nPress outside to exit";
            popup.open();
        }
    }

    // popup to display the clicked coordinates
    Popup {
        id: popup
        x: 100
        y: 100
        width: parent.width
        height: parent.height / 5
        modal: true
        focus: true
        closePolicy: Popup.CloseOnPressOutside
        Text { id: popup_text }
    }
}
