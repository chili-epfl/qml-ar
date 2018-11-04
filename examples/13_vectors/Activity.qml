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
    id: root
    property alias d: demo

    DemoVectors {
        id: demo
    }
}
