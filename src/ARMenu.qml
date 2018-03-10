import QtQuick 2.6
import AR 1.0

// Hidden menu for AR module
import QtQuick.Controls 1.5

Item {
    property var activity

    id: root

    Rectangle {
        id: blobs_menu
        anchors.left: parent.left
        anchors.top: parent.top
        width: root.width * 0.33
        height: parent.height * 0.5
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
        height: parent.height * 0.5
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
        height: parent.height * 0.5
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

    Rectangle {
        id: filtering_param
        anchors.left: parent.left
        anchors.top: fps_menu.bottom
        width: root.width * 0.3
        height: parent.height * 0.5

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Helvetica"
            font.bold: true
            text: "High-Pass " + Math.round(filtering_param_slider_.value * 100) + "%"
            font.weight: Font.Normal
        }
    }

    Rectangle {
        id: filtering_param_slider
        anchors.left: filtering_param.right
        anchors.top: fps_menu.bottom
        width: root.width * 0.7
        height: parent.height * 0.5

        Slider {
            anchors.fill: parent
            id: filtering_param_slider_
            activeFocusOnPress: false
            tickmarksEnabled: true
            minimumValue: 0.0
            stepSize: 0.01
            maximumValue: 1.0
            value: 1.0
            styleHints: 1
            onValueChanged: {AR.filter_alpha = value;}
        }
    }
}
