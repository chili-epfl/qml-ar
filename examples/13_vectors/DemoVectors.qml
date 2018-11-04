import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick 2.0

// Show some vectors using LocalizedVector and ARArrow
Entity {
    id: root
    property var lst: [arrow_x, arrow_y, arrow_z]

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

    LocalizedVector {
        id: vector_demo_z
        from: Qt.vector3d(0, 0, 0)
        to: Qt.vector3d(0, 0, -30)
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

    ARArrow {
        id: arrow_z
        lvector: vector_demo_z
        color: Qt.rgba(0, 0, 1, 0)
    }
}
