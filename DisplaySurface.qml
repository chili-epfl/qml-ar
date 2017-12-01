import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0 as QQ2

Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionMatrix: detector.projector
    }

    FirstPersonCameraController { camera: camera }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: camera
                clearColor: "transparent"
            }
        },
        InputSettings { }
    ]


    Entity {components: [transform, material, cubeMesh, chest, light]}

    Transform {
        id: transform
        matrix: {
            var m = Qt.matrix4x4();
            m.translate(Qt.vector3d(0, 0, 0));
            return m;
        }
    }
/*
    CuboidMesh {
        id: cubeMesh
        xExtent: 600
        yExtent: 600
        zExtent: 100
    }
    */

    // Point Light (pulsing)
    Entity {
        id: light
        components: [
            Transform {
                translation: Qt.vector3d(0.0, 0, -300)
            },
            PointLight {
                color: "white"
                intensity: 1
                constantAttenuation: 1.0
                linearAttenuation: 0.0
                quadraticAttenuation: 0

                NumberAnimation on intensity {
                    from: 0.8; to: 1;
                    running: true
                    loops: Animation.Infinite
                    duration: 100
                    easing.type: Easing.CosineCurve
                }
            }
        ]
    }

    RenderableEntity {
        id: chest
        source: "/assets/Chest.obj"
        position: Qt.vector3d(0, 0, -600)
        rotationAngle: 0
        scale: 5

        material: DiffuseMapMaterial {
            id: material
            diffuse: TextureLoader { source: "/assets/diffuse.webp" }
            specular: Qt.rgba( 0.5, .5, .5, 1.0 )
            shininess: 2.0
        }
    }

}
