/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Using the image input instead of camera. After first detected marker
 * The application exits (to be used as a test after installing)
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import AR 1.0

/** @brief Load image, detect markers, exit */
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

        // close the app when markers detected
        onMvp_matrixChanged: {
            if(AR.pose_valid && AR.markers.length > 0) {
                console.log('Matrix available, closing the app');
                Qt.quit();
            }
        }
    }
}
