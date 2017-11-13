import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    MainForm {

        anchors.fill: parent
        mouseArea.onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
        Timer {
            interval: 10; running: true; repeat: true
            onTriggered: {parent.image.cache=0;parent.image.source="";parent.image.source="image://cvimg/red"}
        }
    }


}
