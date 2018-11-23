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

    Entity {
        id: vector_list

        // list of arrows
        property var lst: [arrow_x, arrow_y]

        // Demo: x, y, z
        LocalizedVector {
            id: vector_demo_x
            from: Qt.vector3d(0, 0, 0)
            to: Qt.vector3d(30, 0, 0)
        }

        LocalizedVector {
            id: vector_demo_y
            from: Qt.vector3d(0, 0, 0)
            to: Qt.vector3d(0, 30, 0)
        }

        ARArrow {
            id: arrow_x
            lvector: vector_demo_x
            color: Qt.rgba(1, 0, 0, 0)
        }

        ARArrow {
            id: arrow_y
            lvector: vector_demo_y
            color: Qt.rgba(0, 1, 0, 0)
        }
    }
}
