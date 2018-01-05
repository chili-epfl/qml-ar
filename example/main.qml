import QtQuick 2.6
import QtQuick.Window 2.2
import Cellulo 1.0
import QtQuick.Scene3D 2.0
import CelluloAR 1.0
import QtMultimedia 5.4

Window {
    id: window
    visible: true
    title: qsTr("Cellulo-AR")

/*
    Camera {
        id: camera
        deviceId: "1"
        objectName: "camera"
    }
*/

    Component.onCompleted: {
        // Set camera id (Qt/OpenCV backend)
        CelluloAR.camera_id = 0

        // Set image width in pixels
        CelluloAR.image_width = 300

        // Set camera object and install VideoProbe
        //CelluloAR.qml_camera = camera

        // Set image as input
        //CelluloAR.image_filename = "assets/dots.sample.png"

        // Set update frequency
        CelluloAR.update_ms = 100
    }

    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 0

        // Resize window on first valid image
        Timer {
            interval: 100; running: true; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(w * h > 0)
                {
                    console.log("Resizing main window")
                    window.width = w;
                    window.height = h;
                    running = false;
                }
            }
        }

        // Update image each 10 ms
        Timer {
            interval: 10; running: true; repeat: true
            onTriggered: {image.cache=0;image.source="";image.source="image://CelluloARMarkers/raw";}
        }

/*
        VideoOutput {
            id: viewfinder
            source: camera
            autoOrientation: true
            anchors.fill: parent
            anchors.margins: 0
        }
*/

        Image {
            id: image
            anchors.fill: parent
            visible: true
            clip: false
            transformOrigin: Item.Center
            source: ""
        }

        Scene3D {
            id: scene3d
            anchors.fill: parent
            anchors.margins: 0
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            DisplaySurface {
                robotList: window.robotList
            }
        }
    }

    property var robotList : [robot1]

    CelluloBluetooth{
        id: robot1
        macAddr : "00:06:66:74:43:01"
        onMacAddrChanged:           console.log("robot1.onMacAddrChanged()")

        onBootCompleted:            console.log("robot1.bootCompleted()")
        // @disable-check M306
        onConnectionStatusChanged:  console.log("robot1.connectionStatusChanged(" + CelluloBluetoothEnums.ConnectionStatusString(connectionStatus) + ")")
        // @disable-check M306
        onGestureChanged:           console.log("robot1.gestureChanged(" + CelluloBluetoothEnums.GestureString(gesture) + ")")
        onKidnappedChanged:         console.log("robot1.kidnappedChanged(" + kidnapped + ")")
        onPoseChanged:              console.log("robot1.pc(" + x.toFixed(2) + "," + y.toFixed(2) + "," + theta.toFixed(2) + ")")
        onTrackingGoalReached:      console.log("robot1.trackingGoalReached()")
    }


}
