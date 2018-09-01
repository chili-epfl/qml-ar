/**
 * @file Activity.qml
 * @brief This file renders the scene in Qt3D
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

/** @brief This file renders the scene in Qt3D */
Entity {
    id: activity

    /** @brief Chest x position (in mm) */
    property var chest_x

    /** @brief Chest y position (in mm) */
    property var chest_y

    // cuboid on top-left corner
    Entity {
        id: cuboid
        components: [
            PhongMaterial {
                id: materialPhong
            },

            // @disable-check M300
            Transform {
                id: transform
                matrix: {
                    var m = Qt.matrix4x4();
                    m.translate(Qt.vector3d(0, 0, 0));
                    return m;
                }
            },
            CuboidMesh {
                id: cubeMesh

                // size of the cuboid
                xExtent: 50
                yExtent: 50
                zExtent: 10
            }
        ]
    }

    // Point Light (constant)
    Entity {
        id: light1
        components: [
            // @disable-check M300
            Transform {
                translation: Qt.vector3d(0.0, 100, -100)
            },
            PointLight {
                color: "white"
                intensity: 2
                constantAttenuation: 1.0
                linearAttenuation: 0.0
                quadraticAttenuation: 0
            }
        ]
    }

    // chest on bottom-down corner
    RenderableEntity {
        id: chest
        // Mesh
        source: "/assets/Chest.obj"
        position: Qt.vector3d(activity.chest_x, activity.chest_y, 0)
        rotationAngle: -90
        scale: 1

        material: DiffuseMapMaterial {
            id: material
            // @disable-check M16
            diffuse: TextureLoader { source: "/assets/diffuse.webp" } // Texture
            specular: Qt.rgba( 0.5, .5, .5, 1.0 )
            shininess: 2.0
        }
    }
}
