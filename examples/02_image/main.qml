import QtQuick.Window 2.2
import AR 1.0

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

        // close the app if markers detected
        onMvp_matrixChanged: {
            if(AR.pose_valid && AR.markers.length > 0) {
                console.log('Matrix available, closing the app');
                Qt.quit();
            }
        }
    }
}
