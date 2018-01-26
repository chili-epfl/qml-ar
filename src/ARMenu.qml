import QtQuick 2.6

// Hidden menu for AR module

Item {
    property var activity

    id: root

    Rectangle {
        id: blobs_menu
        anchors.left: parent.left
        anchors.top: parent.top
        width: 100
        height: 100
        color: "green"
        opacity: 0.5

        Text {
            anchors.fill: parent
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
        width: 100
        height: 100
        color: "green"
        opacity: 0.5

        Text {
            anchors.fill: parent
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
        width: 100
        height: 100
        color: "green"
        opacity: 0.5

        Text {
            anchors.fill: parent
            text: "FPS"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: root.activity.show_fps = !root.activity.show_fps
        }
    }
}
