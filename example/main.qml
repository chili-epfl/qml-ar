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

    Camera {
        id: camera
        deviceId: "1"
        objectName: "camera"
    }

    Component.onCompleted: {
        //CelluloAR.camera_id = 0
        CelluloAR.qml_camera = camera
        CelluloAR.update_ms = 50
        //CelluloAR.image_filename = "assets/dots.sample.png"
    }

    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 0

        VideoOutput {
            id: viewfinder
            source: camera
            autoOrientation: true
            anchors.fill: parent
            anchors.margins: 0
        }

        Scene3D {
            id: scene3d
            anchors.fill: parent
            anchors.margins: 0
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            DisplaySurface {
                robotList: robotListId
            }
        }
    }

    property var robotListId : [robot1]

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
