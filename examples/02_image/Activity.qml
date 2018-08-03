/**
 * @file Activity.qml
 * @brief This file renders the scene in Qt3D
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import Qt3D.Core 2.0
import Qt3D.Extras 2.0

/** @brief a cube at (0, 0, 0) */
Entity {
    components: [
        // adding material
        PhongMaterial { ambient: Qt.rgba(0.5, 0.5, 0.5, 1); },

        // adding 5cm^3 cube
        CuboidMesh { xExtent: 50; yExtent: 50; zExtent: 50 }
    ]
}
