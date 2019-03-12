import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.9

// Show an arrow in AR
Entity {
    id: arrow

    // text to show
    property string text: "ABACABA"

    // do show?
    property bool visible: true

    property vector3d position: Qt.vector3d(0, 0, 0)

    // scaling factor (thickness in mm)
    property real scale: 10

    // rotation (I) around Z
    property real yaw_rad: 0

    // rotation (II) around Y'
    property real roll_deg: Math.PI / 2

    // color of the text
    property color color: Qt.rgba( 1, 1, 0, 1.0 )

    // render
    components: [
        // @disable-check M300
        Transform {
            // do two rotations at the same time (around Y')
            rotation: fromAxesAndAngles(Qt.vector3d(0, 1, 0), 180, Qt.vector3d(0, 0, 1), 90)

            // scale and visible implementation
            scale3D: visible ? Qt.vector3d(arrow.scale, arrow.scale, arrow.scale) :
                               Qt.vector3d(0, 0, 0)

            // translate to 'from'
            translation: position
        },

        PhongMaterial {
            // material for the arrow
            id: mat1

            // react to external light
            shininess: 1.0

            // color implementation
            ambient: arrow.color
        },

        ExtrudedTextMesh {
            depth: 0.1
            text: arrow.text
        }
    ]

    //Component.onCompleted: logger.log(lvector + " " + scale + " " + vector + " " + yaw_rad + " " + roll_deg)
}
