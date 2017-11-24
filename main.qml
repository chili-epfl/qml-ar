import QtQuick 2.6
import QtQuick.Window 2.2
import Cellulo 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt + OpenCV test")

    MainForm {

        anchors.fill: parent

        Timer {
            interval: 100; running: true; repeat: true
            onTriggered: {parent.image.cache=0;parent.image.source="";parent.image.source="image://camera/raw"}
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
