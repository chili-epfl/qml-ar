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

    Entity {
        id: torusEntity
        components: [ material, transform, cubeMesh ]
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
}
