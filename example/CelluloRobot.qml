import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

Entity {
    property var x
    property var y
    property var theta

    components: [
        PhongMaterial {
        },
        // @disable-check M300
        Transform {
            rotation: fromAxisAndAngle(Qt.vector3d(0, 0, 1), theta)
            translation: Qt.vector3d(x, y, -50);
        },
        CuboidMesh {
            xExtent: 50
            yExtent: 2
            zExtent: 2
        }
    ]
}
