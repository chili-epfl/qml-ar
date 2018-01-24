import QtQuick 2.6
import QtQuick.Window 2.2
import AR 1.0
import QtMultimedia 5.9
import QtQml 2.2

Window {
    id: window
    visible: true
    title: qsTr("QML-AR")

    minimumWidth: arComponent.width
    minimumHeight: arComponent.height

    // crearing AR component
    ARComponent {
        id: arComponent

        // use camera
        init_type: AR.INIT_CAMERA

        // scale image to 600px width
        image_width: 480

        // load scene
        arSceneComponent: Qt.createComponent("Activity.qml")

        // add chest coordinates as parameters
        arSceneParameters: {'chest_x': 210, 'chest_y': 297}

        // set to true to force width to this value
        force_width: true
        width: 1500

        // do show fps?
        show_fps: true

        // set output type
        output_type: AR.OUTPUT_MARKERS

        // show blobs?
        show_blobs: false

        // show markers?
        show_markers: false
    }
}
