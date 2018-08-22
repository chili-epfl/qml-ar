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

    /** Translation of the cube (x, y, z) */
    property vector3d translation: Qt.vector3d(500, 500, 0)

    components: [
        // adding a translation
        // @disable-check M300
        Transform {
            translation: root.translation
        },

        PerVertexColorMaterial {},

        // adding 5cm^3 cube
        CuboidMesh { xExtent: 1000; yExtent: 1000; zExtent: 10 }
    ]
}
