import QtQuick 2.6
import QtQuick.Window 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Image { source: "image://cvimg/red" }

    MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            console.log(qsTr('Clicked on background. Text: "' + textEdit.text + '"'))
        }
    }

    Timer {
        interval: 500; running: true; repeat: true
        onTriggered: ;//        }//backend.updateImage()//console.log(Date().toString())
    }
}
