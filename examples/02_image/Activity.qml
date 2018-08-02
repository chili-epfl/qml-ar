import Qt3D.Core 2.0
import Qt3D.Extras 2.0

// a cube at (0, 0, 0)
Entity {
    components: [
        // adding material
        PhongMaterial { ambient: Qt.rgba(1, 1, 1, 1); },

        // adding 5cm^3 cube
        CuboidMesh { xExtent: 50; yExtent: 50; zExtent: 50 }
    ]
}
