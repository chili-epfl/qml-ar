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

    property real robotx: 100
    property real roboty: 100
    property real robot_theta: 0

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

    // cellulo robot visual
    Entity {
        id: celluloDirection
        components: [
            PhongMaterial {
            },
            Transform {
                rotation: fromAxisAndAngle(Qt.vector3d(0, 0, 1), sceneRoot.robot_theta)
                translation: Qt.vector3d(sceneRoot.robotx, sceneRoot.roboty, -100);
            },
            CuboidMesh {
                xExtent: 50
                yExtent: 2
                zExtent: 2
            }
        ]
    }

    Entity {
        id: cuboid
        components: [
            PhongMaterial {
                id: materialPhong
            },
            Transform {
                id: transform
                matrix: {
                    var m = Qt.matrix4x4();
                    m.translate(Qt.vector3d(0, 0, -100));
                    return m;
                }
            },
            CuboidMesh {
                id: cubeMesh
                xExtent: 150
                yExtent: 150
                zExtent: 0.15
            }
        ]
    }


    // Point Light (pulsing)
    Entity {
        id: light
        components: [
            Transform {
                translation: Qt.vector3d(0.0, 0, 150)
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
                    duration: 1000
                    easing.type: Easing.CosineCurve
                }
            }
        ]
    }

    // Point Light (constant)
    Entity {
        id: light1
        components: [
            Transform {
                translation: Qt.vector3d(0.0, 100, -225)
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

    RenderableEntity {
        id: chest
        source: "/assets/Chest.obj"
        position: Qt.vector3d(210, 297, -150)
        rotationAngle: 0
        scale: 1

        material: DiffuseMapMaterial {
            id: material
            diffuse: TextureLoader { source: "/assets/diffuse.webp" }
            specular: Qt.rgba( 0.5, .5, .5, 1.0 )
            shininess: 2.0
        }
    }

}
