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

    /** Return the minimal value */
    function min(arr) {
        var imin = 0;
        for(var i = 0; i < arr.length; i++) {
            if(arr[i] < arr[imin]) {
                imin = i;
            }
        }
        return arr[imin];
    }

    /** Return the index with minimal value */
    function argmin(arr) {
        var imin = -1;
        for(var i = 0; i < arr.length; i++) {
            if(imin == -1 || arr[i] < arr[imin]) {
                imin = i;
            }
        }
        return imin;
    }

    /** Return the index with minimal value */
    function argminFcn(arr, fcn) {
        var imin = -1;
        for(var i = 0; i < arr.length; i++) {
            if(imin == -1 || fcn(arr[i]) < fcn(arr[imin])) {
                imin = i;
            }
        }
        return imin;
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

        // the selected item to change in the lst array
        property int selected: -1

        // which point to move?
        // "start", "end", "middle"
        property int type: 0

        // the original position for the point selected, type
        property vector3d lastPoint: Qt.vector3d(0, 0, 0)

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            mouseHover = true;

            // z position of the click
            var z_mm = 0;

            // closest vector
            var closest_i = -1;

            // found the vector?
            var found = 0;

            // current click point
            var vec = Qt.vector3d(x_mm, y_mm, z_mm);

            // threshold for which the click is registered
            var threshold = 5;

            // list of arArrows
            var lst = arSceneObject.lst;

            function distances(arrow) {
                // from, to pts
                var from   = arrow.lvector.from;
                var to     = arrow.lvector.to;
                var middle = arrow.lvector.middle;

                // distance to FROM
                var d_from   = from.minus(vec).length();
                var d_to     = to.minus(vec).length();
                var d_middle = middle.minus(vec).length();

                // distances: from, to, middle
                var dst = [d_from, d_to, d_middle];

                return dst;
            }

            // closest arrow point
            var closest_arrow = argminFcn(lst, function (arrow) {
                // return minimal distance to best arrow
                return min(distances(arrow));
            });

            console.log("Closest arrow: ", closest_arrow);

            // closest arrow
            var arrow = lst[closest_arrow];

            // distances to that arrow
            var dst = distances(arrow);

            // the closest point is too far, doing nothing
            if(min(dst) > threshold) {
                return;
            }

            selected = closest_arrow;
            type = argmin(dst);
            lastPoint = arrow.lvector.points[type];

            console.log(selected, type, lastPoint);
        }

        onMovedOnActivity: {
            return;
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
