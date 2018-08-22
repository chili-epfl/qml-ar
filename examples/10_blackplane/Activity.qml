/**
 * @file Activity.qml
 * @brief This file renders the scene in Qt3D
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0

/** @brief a cube at some (x, y, z) */
Entity {
    id: root

    /** Make big/small */
    property bool isBig: false

    /** Side of the cuboid */
    property real scale: isBig ? 50 : 2

    components: [
        // adding a scale
        // @disable-check M300
        Transform {
            scale: root.scale
            translation: Qt.vector3d(210. / 2, 297. / 2, 0)
            rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 90)
        },

        PerVertexColorMaterial {},

        Mesh {
            id: mesh
            source: "/assets/cuboid.obj"
        }
    ]
}
