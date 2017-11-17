import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Qt + OpenCV test")

    MainForm {

        anchors.fill: parent
        Timer {
            interval: 20; running: true; repeat: true
            onTriggered: {parent.image.cache=0;parent.image.source="";parent.image.source="image://camera/raw"}
        }
    }


}
