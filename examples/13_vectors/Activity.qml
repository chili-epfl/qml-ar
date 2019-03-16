/**
 * @file Activity.qml
 * @brief This file renders the scene in Qt3D
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick 2.6
import Qt3D.Core 2.0
import Qt3D.Extras 2.0

/** @brief a cube at (0, 0, 0) */
Entity {
    id: root

    /** List of ARArrows */
    property alias lst: vector_list.lst

    // displaying a polygon
    Polygon {
        id: poly
        vertices: [[100, 100, 0], [200, 100, 0], [200, 200, 0], [100, 200, 0]]
        Component.onCompleted: poly.update()
    }

    // entity with some vectors
    Entity {
        id: vector_list

        // list of arrows
        property var lst: []
    }
}
