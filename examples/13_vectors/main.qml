/**
 * @file main.qml
 * @brief Draw vectors and move them on a virtual canvas
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import ch.epfl.chili.qmlar 1.0
import QtQuick 2.6
import QtQuick.Controls 2.3

/** @brief Draw vectors and move them on a virtual canvas */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500
    id: root

    Row {
        width: parent.width
        Button {
            width: 50
            text: "Add"
        }

        Text {
            width: 400
            wrapMode: Text.WrapAnywhere
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: 20
            text: "Click in the middle of a vector to move it, click at the beginning or end to edit that point"
        }

        Button {
            width: 50
            text: "Clear"
        }
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

        property vector2d clickPoint: Qt.vector2d(0, 0)
        property int selected: -1

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            mouseHover = true;

            var z_mm = 0;
            var closest_i = -1;
            var found = 0;

            var vec = Qt.vector3d(x_mm, y_mm, z_mm);
            var threshold = 5;

            var lst = arSceneObject.lst;

            for(var i = 0; i < lst.length; i++)
            {
                var arrow = lst[i];
                var from = arrow.lvector.from;
                var to = arrow.lvector.to;
                console.log(from.minus(vec).length());
                if(from.minus(vec).length() <= threshold)
                {
                    console.log("Clicked at FROM", i);
                }
                else if(to.minus(vec).length() <= threshold)
                {
                    console.log("Clicked at TO", i);
                    clickPoint = Qt.vector2d(x_mm, y_mm)
                    selected = i
                }
            }
        }

        onMovedOnActivity: {
            var delta = Qt.vector3d(x_mm - clickPoint.x, y_mm - clickPoint.y, 0);
            if(selected >= 0)
            {
                arSceneObject.lst[selected].lvector.to = arSceneObject.lst[selected].lvector.to.plus(delta);
                arSceneObject.lst[selected].lvector.from = arSceneObject.lst[selected].lvector.from.plus(delta);
            }

            clickPoint = Qt.vector2d(x_mm, y_mm)
            //arSceneObject.lst[0].lvector.to = Qt.vector3d(x_mm, y_mm, 0);
        }

        init_type: AR.INIT_IMAGE
        image_filename: "://assets/ar_demo_sheet.png"
    }
}
