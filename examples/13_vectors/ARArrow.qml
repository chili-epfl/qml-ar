import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

// Show an arrow in AR
Entity {
    id: arrow

    // do show?
    property bool visible: true

    // vector A->B
    property LocalizedVector lvector

    // difference vector
    property vector3d vector: lvector.vector

    // scaling factor (thickness in mm)
    property real scale: 10

    // rotation (I) around Z
    property real yaw_rad: Math.atan2(vector.y, vector.x)

    // rotation (II) around Y'
    property real roll_deg: 90 - Math.atan2(vector.z,
                                            lvector.xypart.length())
                            * 180 / Math.PI

    // color of the arrow
    property color color: Qt.rgba( 1, 1, 0, 1.0 )

    function scaleColor(c, howMuch) {
        return Qt.rgba(c.r * howMuch, c.g * howMuch, c.b * howMuch, c.a);
    }

    // start/end text
    ARText {
        position: lvector.from
        text: lvector.fromText
        color: arrow.color
        visible: arrow.visible
    }

    ARText {
        position: lvector.to
        text: lvector.toText
        color: arrow.color
        visible: arrow.visible
    }

    Entity {
        // render
        components: [
            // @disable-check M300
            Transform {
                // do two rotations at the same time (around Y')
                rotation: fromAxisAndAngle(Qt.vector3d(
                                               -Math.sin(yaw_rad),
                                               Math.cos(yaw_rad),
                                               0),
                                           roll_deg)

                // scale and visible implementation
                scale3D: visible ? Qt.vector3d(arrow.scale, arrow.scale, vector.length() / 5) :
                                   Qt.vector3d(0, 0, 0)

                // translate to 'from'
                translation: lvector.from
            },

            PhongMaterial {
                // material for the arrow
                id: mat1

                // react to external light
                shininess: 1.0

                // color implementation
                ambient: scaleColor(arrow.color, lvector.colorScale)
            },

            Mesh {
                // points to +z, depth [0,5], width=height=[-0.5, 0.5]
                source: "/assets/arrow.obj"
            }
        ]
    }

    //Component.onCompleted: logger.log(lvector + " " + scale + " " + vector + " " + yaw_rad + " " + roll_deg)
}
