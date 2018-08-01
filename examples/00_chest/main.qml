import QtQuick 2.6
import QtQuick.Window 2.2
import AR 1.0
import QtMultimedia 5.9
import QtQml 2.2
import QtQuick.Controls 2.3

Window {
    id: window
    visible: true
    title: qsTr("QML-AR")

    minimumWidth: arComponent.width
    minimumHeight: arComponent.height

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
        width: avr_mode ? 1200 : 1500

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

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            popup_text.text = "Clicked at x = " + Math.round(x_mm, 1) + "mm y = " + Math.round(y_mm, 1) + "mm z = 0mm.\nPress outside to exit";
            popup.open();
        }

        // write a message to console when marker visibility changes
        onMarkers_visibleChanged: {
            console.log('Markers visible? ' + markers_visible)
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
           Text {
               id: popup_text
           }
       }
}
