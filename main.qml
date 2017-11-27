import QtQuick 2.6
import QtQuick.Window 2.2
import Cellulo 1.0
import QtQuick.Scene3D 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt + OpenCV test")

    Rectangle {

        Timer {
            interval: 100; running: true; repeat: true
            onTriggered: {image.cache=0;image.source="";image.source="image://camera/raw"}
        }

        id: scene
        anchors.fill: parent
        anchors.margins: 0
        color: "darkRed"

        transform: Rotation {
            id: sceneRotation
            axis.x: 1
            axis.y: 0
            axis.z: 0
            origin.x: scene.width / 2
            origin.y: scene.height / 2
        }

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

            DisplaySurface {}
        }
    }

    property var robotListId : [robot1]

    CelluloBluetooth{
        id: robot1
        macAddr : "00:06:66:74:40:D4"
        onMacAddrChanged:           console.log("robot1.onMacAddrChanged()")

        onBootCompleted:            console.log("robot1.bootCompleted()")
        onConnectionStatusChanged:  console.log("robot1.connectionStatusChanged(" + CelluloBluetoothEnums.ConnectionStatusString(connectionStatus) + ")")
        onGestureChanged:           console.log("robot1.gestureChanged(" + CelluloBluetoothEnums.GestureString(gesture) + ")")
        onKidnappedChanged:         console.log("robot1.kidnappedChanged(" + kidnapped + ")")
        onPoseChanged:              console.log("robot1.pc(" + x.toFixed(2) + "," + y.toFixed(2) + "," + theta.toFixed(2) + ")")
        onTrackingGoalReached:      console.log("robot1.trackingGoalReached()")
    }


}
