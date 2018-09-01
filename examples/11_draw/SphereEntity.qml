import Qt3D.Core 2.0
import Qt3D.Extras 2.0

// draw a sphere with desired parameters
Entity {
    id: root
    property real x: 0
    property real y: 0
    property real z: -r / 2
    property real r: 5

    components: [
        // @disable-check M300
        Transform { translation: Qt.vector3d(x - r / 2, y - r / 2, z) },

        // adding material
        PhongMaterial { ambient: Qt.rgba(0.5, 0.5, 0.5, 1); },

        // adding 5cm^3 cube
        SphereMesh { radius: root.r }
    ]
}
