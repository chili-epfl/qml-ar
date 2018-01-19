import QtQuick 2.6
import AR 1.0
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4 as QQC
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
    visible: true
    anchors {
        horizontalCenter: parent.horizontalCenter
        verticalCenter: parent.verticalCenter
    }

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

    // will contain Scene3D component
    property var scene3d

    // set to true to preserve width attribute
    // and set height so that image ratio is preserved
    property bool force_width: false

    // do show fps?
    property bool show_fps: true

    // initial width and height
    width: 100
    height: 100

    // initialize AR component on loading
    // properties must be set beforehand
    Component.onCompleted: {
        // Set image width in pixels
        AR.image_width = arComponent.image_width
        console.log("Set image width to " + arComponent.image_width);

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
            image_timer.running = true;
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

    function load_scene3d() {
        console.log("Begin loading scene3d");
        var component = Qt.createComponent("AR3DScene.qml");
        if(component.status === Component.Error) {
            console.debug("Error loading scene: " + component.errorString());
        }
        else {
            scene3d = component.createObject(scene, {'arSceneComponent': arSceneComponent,
                                                 'arSceneParameters': arSceneParameters});
            arSceneObject = scene3d.arSceneObject;
            loading.visible = false;
        }
        console.log("End loading scene3d");
    }

    // scene which displays camera image and OpenGL scene
    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 0

        // indicator showing loading circle
        QQC.BusyIndicator {
            id: loading
            running: true
            visible: true
            anchors.fill: parent
        }

        // Resize AR component on first valid image
        Timer {
            interval: 100; running: true; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(w * h > 1)
                {
                    console.log("Image size is " + w + " x " + h)
                    if(force_width) {
                        arComponent.height = 1. * arComponent.width * h / w;
                    }
                    else {
                        arComponent.width = w;
                        arComponent.height = h;
                    }
                    running = false;
                    image.visible = true;

                    // load scene on component loading
                    arComponent.load_scene3d();
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
            visible: false
            clip: false
            transformOrigin: Item.Center
        }

        // update FPS on new image
        Connections {
            target: AR
            onImageUpdated: fps_text.update()
        }

        // text showing FPS
        Text {
            id: fps_text
            color: "red"
            font.pointSize: 20

            visible: arComponent.show_fps

            // last ms of image update
            property real last_update: 0

            // filtered fps mean
            property real fps_mean: 0

            // last time delta ms
            property real last_delta: 0

            // smoothing parameter
            property real lambda: 0.9

            text: "FPS: " + Math.round(fps_mean) + "; delta: " + Math.round(last_delta) + " ms"
            anchors.left: parent.left
            anchors.top: parent.top


            // call on new frame
            function update() {
                // obtaining ms
                var this_update = new Date().getTime();

                // if not first launch, update text
                if(fps_text.last_update > 0) {
                    // difference since prev. update
                    var difference = this_update - fps_text.last_update;

                    // FPS = 1000 / delta_ms
                    var this_fps = 1000. / difference;

                    // setting last delta
                    fps_text.last_delta = difference;

                    // lambda smoothing
                    if(fps_text.fps_mean > 0) {
                        fps_text.fps_mean = fps_text.lambda * fps_text.fps_mean + (1 - fps_text.lambda) * this_fps;
                    }
                    else {
                        fps_text.fps_mean = this_fps;
                    }
                }

                // updating last_update
                fps_text.last_update = this_update;
            }
        }
    }
}
