import QtQuick 2.6
import AR 1.0
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

// 3D scene which uses OpenGL
Scene3D {
    id: scene3d
    //anchors.fill: parent
    //anchors.margins: 0
    focus: true
    visible: true
    aspects: ["input", "logic"]
    cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

    // set this to the actual surface with 3D models
    // will create an object of this component
    // and add this to ar scene
    property var arSceneComponent

    // parameters for the arScene
    property var arSceneParameters

    // the resulting object will be stored here
    property var arSceneObject

    // delta MVP x axis (in ???)
    property double delta_x: 0

    // entity with camera which creates user's components inside
    Entity {
        id: activity

        // add camera as a component
        components: [
            RenderSettings {
                activeFrameGraph: ForwardRenderer {
                    camera: camera
                    clearColor: "transparent"
                }
            },
            InputSettings { }
        ]

        // set ModelViewProjection matrix as camera matrix
        Camera {
            id: camera
            projectionMatrix: AR.p_matrix.times(Qt.matrix4x4(1, 0, 0, delta_x,
                                                             0, 1, 0, 0,
                                                             0, 0, 1, 0,
                                                             0, 0, 0, 1)).times(AR.mv_matrix)
        }

        // load scene on component loading
        Component.onCompleted: {
            console.log("Begin loading scene " + arSceneComponent + " with params " + arSceneParameters);
            if(!arSceneComponent) {
                console.log("No scene found (null object)");
            }
            else {
                if(arSceneComponent.status === Component.Error) {
                    console.debug("Error loading scene: " + arSceneComponent.errorString());
                }
                else {
                    arSceneObject = arSceneComponent.createObject(activity, arSceneParameters);
                }
            }
            console.log("End loading scene");
        }
    }
}
