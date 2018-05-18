import QtQuick 2.10
import QtQuick.Window 2.10
import AR 1.0

Window {
    visible: true
    width: 640
    height: 480
    id: root
    title: qsTr("Two AR activities example")

    Rectangle {
        id: ar1
        width: arComponent.width
        height: arComponent.height
        anchors.top: root.top
        anchors.left: root.left
        // crearing AR component
        ARComponent {
            id: arComponent

            // enable Augmented Virtual Reality
            // adds two video outputs with one eye shifted
            // relative to another
            //avr_mode: true
            avr_mode: false

            // use camera
            init_type: AR.INIT_CAMERA

            // scale image to 600px width
            image_width: 640

            // load scene
            arSceneComponent: Qt.createComponent("Activity.qml")

            // add chest coordinates as parameters
            arSceneParameters: {'chest_x': 210, 'chest_y': 297}

            // set to true to force width to this value
            force_width: true

            // set width to 1500 on tablet (regular mode)
            // and to 1200 on phone (AVR mode)
            width: 300

            // do show fps?
            show_fps: true

            // set output type
            output_type: AR.OUTPUT_CAMERA

            // show blobs?
            show_blobs: false

            // show markers?
            show_markers: false

            // use image instead of viewfinder
            force_image: false
        }
    }

    Rectangle {
        id: ar2
        width: arComponent.width
        height: arComponent.height
        anchors.top: root.top
        anchors.left: ar1.right
        // crearing AR component
        ARComponent {
            id: arComponent1

            // enable Augmented Virtual Reality
            // adds two video outputs with one eye shifted
            // relative to another
            //avr_mode: true
            avr_mode: false

            // use camera
            init_type: AR.INIT_CAMERA

            // scale image to 600px width
            image_width: 640

            // load scene
            arSceneComponent: Qt.createComponent("Activity.qml")

            // add chest coordinates as parameters
            arSceneParameters: {'chest_x': 210, 'chest_y': 297}

            // set to true to force width to this value
            force_width: true

            // set width to 1500 on tablet (regular mode)
            // and to 1200 on phone (AVR mode)
            width: 300

            // do show fps?
            show_fps: true

            // set output type
            output_type: AR.OUTPUT_CAMERA

            // show blobs?
            show_blobs: false

            // show markers?
            show_markers: false

            // use image instead of viewfinder
            force_image: false
        }

    }
}
