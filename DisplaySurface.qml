import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0 as QQ2

Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionMatrix: md.Hmatrix
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

    PhongMaterial {
        id: material
    }

    Transform {
        id: transform
        matrix: {
            var m = Qt.matrix4x4();
            m.translate(Qt.vector3d(0.5, 0.5, 1));
            return m;
        }
    }

    CuboidMesh {
        id: cubeMesh
        xExtent: 1
        yExtent: 2
        zExtent: 0.001
    }

/*
    RenderableEntity {
        id: chest
        source: "/assets/Chest.obj"
        position: Qt.vector3d(0, 0, 0)
        scale: 1

        material: DiffuseMapMaterial {
            id: material
            diffuse: TextureLoader { source: "/assets/diffuse.webp" }
            specular: Qt.rgba( 0.2, 0.2, 0.2, 1.0 )
            shininess: 2.0
        }
    }
*/
}
