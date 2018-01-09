import QtQuick 2.6
import QtQuick.Window 2.2
import Cellulo 1.0
import CelluloAR 1.0

Window {
    id: window
    visible: true
    title: qsTr("Cellulo-AR")

    // crearing AR component
    CelluloARComponent {
        id: arComponent

        // use camera
        init_type: CelluloAR.INIT_CAMERA

        // scale image to 600px width
        image_width: 600

        // load scene
        arSceneComponent: Qt.createComponent("CelluloActivity.qml")

        // add robot as scene parameter
        arSceneParameters: {"robotList": [robot1]}
    }

    // robot connection
    CelluloBluetooth {
        id: robot1
        macAddr : "00:06:66:74:48:A7"
        onMacAddrChanged:           console.log("robot1.onMacAddrChanged()")
        onBootCompleted:            console.log("robot1.bootCompleted()")
        // @disable-check M306
        onConnectionStatusChanged:  console.log("robot1.connectionStatusChanged(" + CelluloBluetoothEnums.ConnectionStatusString(connectionStatus) + ")")
        // @disable-check M306
        onGestureChanged:           console.log("robot1.gestureChanged(" + CelluloBluetoothEnums.GestureString(gesture) + ")")
        onKidnappedChanged:         console.log("robot1.kidnappedChanged(" + kidnapped + ")")
        onPoseChanged:              {
            console.log("robot1.pc(" + x.toFixed(2) + "," + y.toFixed(2) + "," + theta.toFixed(2) + ")");
            arComponent.arSceneObject.updatePositions();
        }
        onTrackingGoalReached:      console.log("robot1.trackingGoalReached()")
    }
}
