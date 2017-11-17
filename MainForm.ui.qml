import QtQuick 2.6

Rectangle {
    property alias mouseArea: mouseArea

    width: 360
    height: 360
    property alias image: image

    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }

    Image {
        id: image
        anchors.fill: parent
        visible: true
        clip: false
        transformOrigin: Item.Center
        source: ""
    }
}
