import QtQuick 2.6
import CelluloAR 1.0
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

/*
 * This component implements the QML interface to the CelluloAR library
 * Usage:
 * 1. Set init_type (at creation)
 * 2. Set camera_id/image_filename (at creation)
 * 3. Set other parameters at creation
 * 4. Set arScene (string path to qml file) with arSceneParameters
 */

Item {
    id: arComponent

    // initialization type (camera/image)
    property int init_type: CelluloARInitTypes.Camera

    // id of the camera to use
    property int camera_id: -1

    // path to image to open
    property string image_filename: "assets/dots.sample.png"

    // scale image to this width
    property int image_width: 600

    // update image each update_ms milliseconds
    property int update_ms: 100

    // set this to the actual surface with 3D models
    // will load this component and add this to ar scene
    property string arScene

    // parameters for the arScene
    property var arSceneParameters

    // initialize AR component on loading
    // properties must be set beforehand
    Component.onCompleted: {
        // Set image width in pixels
        CelluloAR.image_width = arComponent.image_width
        console.log("Set image width to " + arComponent.image_width);

        // Set update frequency
        CelluloAR.update_ms = arComponent.update_ms
        console.log("Set update_ms to " + arComponent.update_ms);

        // initialize
        switch(init_type) {
            case CelluloARInitTypes.Camera:
                // from camera id
                CelluloAR.camera_id = arComponent.camera_id;
                console.log("Using camera id " + arComponent.camera_id);
                break;
            case CelluloARInitTypes.Image:
                // from image
                CelluloAR.image_filename = arComponent.image_filename;
                console.log("Using image " + arComponent.image_filename);
                break;
            case CelluloARInitTypes.QMLCamera:
                // Set camera object and install VideoProbe
                //CelluloAR.qml_camera = camera
                break;
            default:
                // unknown value
                console.error("Please set valid init type");
        }
    }

    // scene which displays camera image and OpenGL scene
    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 0

        // Resize window on first valid image
        Timer {
            interval: 100; running: true; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(w * h > 0)
                {
                    console.log("Resizing main window")
                    window.width = w;
                    window.height = h;
                    running = false;
                }
            }
        }

        // Update image each 10 ms
        Timer {
            interval: 10; running: true; repeat: true
            onTriggered: {image.cache=0;image.source="";image.source="image://CelluloARMarkers/raw";}
        }

        // image with camera image
        Image {
            id: image
            anchors.fill: parent
            visible: true
            clip: false
            transformOrigin: Item.Center
            source: ""
        }

        // 3D scene which uses OpenGL
        Scene3D {
            id: scene3d
            anchors.fill: parent
            anchors.margins: 0
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

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
                    projectionMatrix: CelluloAR.mvp_matrix
                }

                // load scene on component loading
                Component.onCompleted: {
                    console.log("Begin loading scene " + arScene + " with params " + arSceneParameters);
                    var component = Qt.createComponent(arScene);
                    component.createObject(activity, arSceneParameters);
                    console.log("End loading scene");
                }
            }

        }
    }

}
