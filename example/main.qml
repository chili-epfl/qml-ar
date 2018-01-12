import QtQuick 2.6
import QtQuick.Window 2.2
import AR 1.0

Window {
    id: window
    visible: true
    title: qsTr("QML-AR")

    width: arComponent.width
    height: arComponent.height

    // crearing AR component
    ARComponent {
        id: arComponent

        // use camera
        init_type: AR.INIT_CAMERA

        // scale image to 600px width
        image_width: 600

        // load scene
        arSceneComponent: Qt.createComponent("Activity.qml")

        // add chest coordinates as parameters
        arSceneParameters: {'chest_x': 210, 'chest_y': 297}

        // update frequency
        update_ms: 10
    }
}
