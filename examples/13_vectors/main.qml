/**
 * @file main.qml
 * @brief Draw spheres on the scene with your finger
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import ch.epfl.chili.qmlar 1.0
import QtQuick 2.6
import QtQuick.Controls 2.3


/** @brief Draw spheres on the scene with your finger */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500
    id: root

    Text {
        anchors.top: parent.top
        text: "Click on the scene to add a sphere at that point"
    }

    Button {
        text: "Clear"
        anchors.top: parent.top
        anchors.right: parent.right
        onClicked: arComponent.clearSpheres()
    }

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // no menu in this demo
        disable_menu: true

        // setting width
        width: 500

        // added spheres
        property var spheres: ([]);

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            var component = Qt.createComponent("SphereEntity.qml");
            var obj = component.createObject(arSceneObject, {'x': x_mm, 'y': y_mm})
            spheres.push(obj);
            arSceneObject.clicked(x_mm, y_mm);
            mouseHover = true;
        }

        onMovedOnActivity: console.log(x_mm, y_mm)

        function clearSpheres() {
            for(var i = 0; i < spheres.length; i++) {
                var sphere = spheres[i];
                sphere.destroy();
            }
            spheres = [];
        }

        init_type: AR.INIT_IMAGE

        image_filename: "/home/sergei/arsheet.jpg"
    }
}
