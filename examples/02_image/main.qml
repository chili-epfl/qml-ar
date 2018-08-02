import QtQuick.Window 2.2
import AR 1.0

Window {
    // some window parameters
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
    }
}
