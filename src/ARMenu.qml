import QtQuick 2.6

// Hidden menu for AR module

Item {
    property var activity

    id: root

    Rectangle {
        id: blobs_menu
        anchors.left: parent.left
        anchors.top: parent.top
        width: root.width * 0.33
        height: parent.height
        color: "green"
        opacity: 0.5

        radius: height*0.4
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Helvetica"
            font.pixelSize: 0.2*parent.height
            font.bold: true
            text: "Blobs"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.activity.show_blobs = !root.activity.show_blobs
        }
    }

    Rectangle {
        id: markers_menu
        anchors.left: blobs_menu.right
        anchors.top: parent.top
        width: root.width * 0.33
        height: parent.height
        radius: height*0.4

        color: "green"
        opacity: 0.5

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Helvetica"
            font.pixelSize: 0.2*parent.height
            font.bold: true
            text: "Markers"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.activity.show_markers = !root.activity.show_markers
        }
    }

    Rectangle {
        id: fps_menu
        anchors.left: markers_menu.right
        anchors.top: parent.top
        width: root.width * 0.33
        height: parent.height
        color: "green"
        opacity: 0.5
        radius: height*0.4

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Helvetica"
            font.pixelSize: 0.2*parent.height
            font.bold: true
            text: "FPS"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.activity.show_fps = !root.activity.show_fps
        }
    }
}
