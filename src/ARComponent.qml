import QtQuick 2.6
import AR 1.0
import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

/*
 * This component implements the QML interface to the QMLAR library
 * Usage:
 * 1. Set init_type (at creation)
 * 2. Set camera_id/image_filename (at creation)
 * 3. Set other parameters at creation
 * 4. Set arSceneComponent (loaded using Qt.createComponent)
 *     with arSceneParameters
 *
 * Coordinates are in millimeters
 * System (QML): x right, y down, origin: top-left
 */

Item {
    id: arComponent
    anchors.fill: parent
    visible: true

    // initialization type (camera/image)
    // See QMLAR.InitTypes enum
    property int init_type: AR.INIT_CAMERA

    // id of the camera to use
    property int camera_id: -1

    // path to image to open
    property string image_filename: "assets/dots.sample.png"

    // scale image to this width
    property int image_width: 600

    // update image each update_ms milliseconds
    property int update_ms: 100

    // set this to the actual surface with 3D models
    // will create an object of this component
    // and add this to ar scene
    property var arSceneComponent

    // parameters for the arScene
    property var arSceneParameters

    // the resulting object will be stored here
    property var arSceneObject

    // initialize AR component on loading
    // properties must be set beforehand
    Component.onCompleted: {
        // Set image width in pixels
        AR.image_width = arComponent.image_width
        console.log("Set image width to " + arComponent.image_width);

        // Setting parent size (will be updated later)
        arComponent.parent.width = arComponent.image_width;
        arComponent.parent.height = arComponent.image_width;

        // Set update frequency
        AR.update_ms = arComponent.update_ms
        console.log("Set update_ms to " + arComponent.update_ms);

        // initialize
        console.log("Set init type to " + arComponent.init_type);
        switch(arComponent.init_type) {
        case AR.INIT_CAMERA:
            // from camera id
            console.log("Using camera id " + arComponent.camera_id);
            AR.camera_id = arComponent.camera_id;

            // starting obtaining images
            image_timer.running = true;
            break;
        case AR.INIT_IMAGE:
            // from image
            console.log("Using image " + arComponent.image_filename)
            AR.image_filename = arComponent.image_filename;

            // starting obtaining images
            image_timer.running = true;;
            break;
        case AR.INIT_QMLCAMERA:
            // Set camera object and install VideoProbe
            //AR.qml_camera = camera
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

        // Resize AR component on first valid image
        Timer {
            interval: 100; running: true; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(w * h > 1)
                {
                    console.log("Resizing AR component")
                    arComponent.parent.width = w;
                    arComponent.parent.height = h;
                    running = false;
                }
            }
        }

        // Update image each 10 ms
        Timer {
            id: image_timer
            interval: 10; running: false; repeat: true
            onTriggered: {image.cache=0; image.source=""; image.source="image://ARMarkers/raw";}
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
                    projectionMatrix: AR.mvp_matrix
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
    }
}
