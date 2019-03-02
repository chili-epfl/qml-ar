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

    // row with controls
    Row {
        id: row
        width: parent.width

        // Add a red vector
        Button {
            width: 50
            text: "Add"
            onClicked: addArrow(Qt.rgba(1, 0, 0, 1))
            background: Rectangle {color: "red"}
        }

        // Add a green vector
        Button {
            width: 50
            text: "Add"
            onClicked: addArrow(Qt.rgba(0, 1, 0, 1))
            background: Rectangle {color: "green"}
        }

        // HowTo
        Text {
            width: 350
            wrapMode: Text.WrapAnywhere
            fontSizeMode: Text.Fit
            minimumPixelSize: 10
            font.pixelSize: 20
            text: "Click in the middle of a vector to move it, click at the beginning or end to edit that point"
        }

        // Clear vectors
        Button {
            width: 50
            text: "Clear"
            onClicked: {
                // deselecting
                arComponent.selected = -1;

                // just making the objects invisible, they are still in memory
                /// @todo this is a memory leak
                indexminFcn(arComponent.arSceneObject.lst, function(elem) {
                    elem.visible = false;
                });

                // flushing the list
                arComponent.arSceneObject.lst = [];
            }
        }
    }

    /** Add an arrow to the list */
    function addArrow(color) {
        var obj = Qt.createComponent("LocalizedVector.qml");
        obj = obj.createObject(root, {'from': Qt.vector3d(100, 0, 0),
                         'to': Qt.vector3d(100, 100, 0), 'editable': true});
        var arrow = Qt.createComponent("ARArrow.qml");
        arrow = arrow.createObject(arComponent.arSceneObject, {'lvector': obj,
                           'color': color})
        arComponent.arSceneObject.lst.push(arrow);
        console.log(arComponent.arSceneObject.lst)
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

    /** Return the element with minimal value */
    function argminFcn(arr, fcn) {
        var imin = -1;
        var elem_min = null;
        for(var i = 0; i < arr.length; i++) {
            if(imin == -1 || fcn(arr[i]) < fcn(arr[imin])) {
                imin = i;
                elem_min = arr[i];
            }
        }
        return elem_min;
    }

    /** Return the index with minimal value */
    function indexminFcn(arr, fcn) {
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
        property vector3d lastPointFrom: Qt.vector3d(0, 0, 0)
        property vector3d lastPointTo: Qt.vector3d(0, 0, 0)

        // z plane with the vectors
        property real z_mm: 0

        // calculate distances to from, to, middle points of the arrow
        function distances(point, lvector) {
            // from, to pts
            var from   = lvector.from;
            var to     = lvector.to;
            var middle = lvector.middle;

            // distance to FROM
            var d_from   = from.minus(point).length();
            var d_to     = to.minus(point).length();
            var d_middle = middle.minus(point).length();

            // distances: from, to, middle
            var dst = [d_from, d_to, d_middle];

            return dst;
        }

        // range 0..n-1
        function range(n) {
            return Array.apply(null, new Array(n)).map(function (_, i) {return i;})
        }

        // do an action when clicked on the plane with the markers
        onClickedOnActivity: {
            // if nothing is selected, select the closest point
            if(selected == -1)
            {
                mouseHover = true;
            }
            // otherwise, de-select current point
            else {
                mouseHover = false;
                selected = -1;
                return;
            }

            // closest vector
            var closest_i = -1;

            // found the vector?
            var found = 0;

            // current click point
            var vec = Qt.vector3d(x_mm, y_mm, z_mm);

            // threshold for which the click is registered
            var threshold = 15;

            // list of arArrows
            var lst = arSceneObject.lst;

            // closest arrow point
            var closest_arrow = indexminFcn(lst, function (arrow) {
                // return minimal distance to best arrow
                return min(distances(vec, arrow.lvector));
            });

            // no arrow at all
            if(closest_arrow === -1) return;

            console.log("Closest arrow: ", closest_arrow);

            // closest arrow
            var arrow = lst[closest_arrow];

            // distances to that arrow
            var dst = distances(vec, arrow.lvector);

            // the closest point is too far, doing nothing
            if(min(dst) > threshold) {
                return;
            }

            // selected point
            selected = closest_arrow;

            // type of the point selected
            type = argmin(dst);

            // the current position of the point
            lastPointFrom = arrow.lvector.from;
            lastPointTo = arrow.lvector.to;

            console.log(selected, type, lastPointFrom, lastPointTo);
        }

        property var lst: arSceneObject.lst

        onMovedOnActivity: {
            // do nothing if nothing is selected
            if(selected == -1) return;

            // current clicked point
            var vec = Qt.vector3d(x_mm, y_mm, z_mm);

            // LocalizedVector instance to change
            var vector = lst[selected].lvector;

            // return minimal distance to start/end of best arrow
            function minFromTo(idx) {
                var vect = lst[idx].lvector;
                var dist = distances(vec, vect);
                return min([dist[0], dist[1]]);
            }

            // closest arrow point for all but current
            var closest_arrow = argminFcn(range(lst.length).filter(function(idx) {return idx !== selected}), minFromTo);

            var snap = 0; // 0 nothing 1 to start 2 to end

            if(closest_arrow >= 0 && type != 2) {
                if(minFromTo(closest_arrow) <= 10)
                {
                    var dst = distances(vec, lst[closest_arrow].lvector)
                    var dpoint = 0;
                    if(dst[0] < dst[1]) // snap to beginning
                    {
                        console.log('Snap to beginning of ', closest_arrow)
                        snap = 1;
                    }
                    else {
                        console.log('Snap to end of ', closest_arrow)
                        snap = 2;
                    }
                }
            }

            console.log('Moving', selected)

            // moving FROM
            if(type == 0 && vector.editable)
            {
                if(snap == 0) {
                    vector.from = vec;
                }
                else if(snap == 1) {
                    vector.from = Qt.binding(function() { return lst[closest_arrow].lvector.from })
                }
                else if(snap == 2) {
                    vector.from = Qt.binding(function() { return lst[closest_arrow].lvector.to })
                }
            }
            // moving TO
            else if(type == 1 && vector.editable)
            {
                if(snap == 0) {
                    vector.to = vec;
                }
                if(snap == 1) {
                    vector.to = Qt.binding(function() { return lst[closest_arrow].lvector.from })
                }
                else if(snap == 2) {
                    vector.to = Qt.binding(function() { return lst[closest_arrow].lvector.to })
                }
            }
            // moving the whole vector
            else if(type == 2)
            {
                var m_to_to = lastPointTo.minus(lastPointFrom).times(0.5)
                vector.from = vec.minus(m_to_to);
                vector.to   = vec.plus(m_to_to);
            }
        }

        // using the image instead of camera
        init_type: AR.INIT_IMAGE
        image_filename: "://assets/ar_demo_sheet.png"
    }
}
