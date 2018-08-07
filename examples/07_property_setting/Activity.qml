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
    property vector3d translation: Qt.vector3d(0, 0, 0)

    components: [
        // adding a translation
        // @disable-check M300
        Transform {
            translation: root.translation
        },

        // adding material
        PhongMaterial { ambient: Qt.rgba(0.5, 0.5, 0.5, 1); },

        // adding 5cm^3 cube
        CuboidMesh { xExtent: 50; yExtent: 50; zExtent: 50 }
    ]

    // Point Light with some default parameters
    // so that cube edges are visible
    Entity {
        id: light1
        components: [
            // @disable-check M300
            Transform { translation: Qt.vector3d(0, 0, -100) },
            PointLight { }
        ]
    }
}
