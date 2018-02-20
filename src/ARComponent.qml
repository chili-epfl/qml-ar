import QtQuick 2.6
import AR 1.0
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4 as QQC
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtMultimedia 5.9 as QMM

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
    id: root
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

    // what to show on the component?
    property int output_type: AR.OUTPUT_MARKERS

    // resulting output url
    property string output_url: ""

    // viewfinder object
    property var viewfinder

    // scale processing_image pixels -> screen pixels
    property real scaleDots: 1.0

    // show blobs?
    property bool show_blobs: true

    // show markers?
    property bool show_markers: true

    // use image even if viewfinder available
    property bool force_image: false

    // initial width and height
    width: 300
    height: 300

    // initialize AR component on loading
    // properties must be set beforehand
    Component.onCompleted: {
        // Set image width in pixels
        AR.image_width = root.image_width
        console.log("Set image width to " + root.image_width);

        // set output url
        switch(root.output_type) {
        case AR.OUTPUT_CAMERA:
            root.output_url = "image://ARMarkers/camera";
            break;
        case AR.OUTPUT_MARKERS:
            root.output_url = "image://ARMarkers/markers";
            break;
        default:
            console.error("Please set valid output type");
        }

        // initialize
        console.log("Set init type to " + root.init_type);
        switch(root.init_type) {
        case AR.INIT_CAMERA:
            // from camera id
            console.log("Using camera id " + root.camera_id);
            AR.camera_id = root.camera_id;
            break;
        case AR.INIT_IMAGE:
            // from image
            console.log("Using image " + root.image_filename)
            AR.image_filename = root.image_filename;
            break;
        default:
            // unknown value
            console.error("Please set valid init type");
            return;
        }

        // loading viewfinder if needed
        if(AR.camera != null) {
            console.log("Using viewfinder output");
            load_viewfinder();
        } else {
            console.log("Using image output");
        }

        // starting obtaining images
        image_timer.running = true;

        // trying to resize the component
        init_timer.running = true;
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

    // create viewfinder component
    function load_viewfinder() {
        console.log("Begin loading viewfinder");
        var component = Qt.createComponent("ARViewfinder.qml");
        if(component.status === Component.Error) {
            console.debug("Error loading viewfinder: " + component.errorString());
        }
        else {
            viewfinder = component.createObject(scene, {'source': AR.camera});

            // starting camera
            AR.startCamera();
        }
        console.log("End loading viewfinder");
    }

    // scene which displays camera image and OpenGL scene
    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.margins: 0

        // indicator showing loading circle
        QQC.BusyIndicator {
            z: 10
            id: loading
            running: true
            visible: false
            anchors.fill: parent
        }

        // Resize AR component on first valid image
        Timer {
            id: init_timer
            interval: 100; running: false; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(w * h > 2)
                {
                    console.log("Image size is " + w + " x " + h)
                    if(force_width) {
                        root.height = 1. * root.width * h / w;
                    }
                    else {
                        root.width = w;
                        root.height = h;
                    }

                    // setting scale parameter
                    root.scaleDots = 1.0 * root.width / w;

                    // need to set up only once
                    running = false;

                    // show image if no viewfinder available
                    if(AR.camera !== null && !root.force_image)
                    {
                        console.log("Hiding image");
                        image.cache = 0;
                        image.source = "";
                        image_timer.running = false;
                        image.visible = false;
                    }
                    else
                    {
                        console.log("Showing image");
                        image.visible = true;
                        if(AR.camera !== null)
                            viewfinder.visible = 0;
                    }

                    // load scene on component loading
                    root.load_scene3d();
                }
            }
        }

        Rectangle {
            id: hiddenMenuEnabler
            z: 10
            opacity: 0
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            width: 100
            height: 100

            MouseArea {
                anchors.fill: parent
                onClicked: hiddenMenu.visible = !hiddenMenu.visible
            }
        }

        ARMenu {
            z: 15
            anchors.top: hiddenMenuEnabler.bottom
            anchors.left: hiddenMenuEnabler.left
            width: root.width * 0.5
            activity: root
            height: root.height * 0.1
            id: hiddenMenu
            visible: false
        }

        ARBlobs {
            id: blobs
            visible: root.show_blobs
            scaleDots: root.scaleDots
            z: 10
        }

        ARMarkers {
            id: markers
            visible: root.show_markers
            scaleDots: root.scaleDots
            z: 11
        }

        // Update image each 10 ms
        Timer {
            id: image_timer
            interval: 10; running: false; repeat: true
            onTriggered: {image.cache = 0; image.source = ""; image.source = root.output_url;}
        }

        // image with camera image
        Image {
            id: image
            anchors.fill: parent
            visible: false
            clip: false
            transformOrigin: Item.Center
        }

        // text showing FPS
        Text {
            z: 10
            id: fps_text
            color: "red"
            font.pointSize: 20

            visible: root.show_fps

            text: "FPS: " + Math.round(AR.FPSMean) + " ± " + Math.round(AR.FPSStd)
            anchors.left: parent.left
            anchors.top: parent.top
        }

        // text showing marker visibility
        Text {
            z: 10
            id: markers_text
            color: AR.markers_visible ? "green" : "red"
            font.pointSize: 20

            visible: root.show_fps

            text: "Markers " + (AR.markers_visible ? "present" : "absent")
            anchors.left: parent.left
            anchors.top: fps_text.bottom
        }
    }
}
