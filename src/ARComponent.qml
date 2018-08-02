/**
 * @file ARComponent.qml
 * @brief This component is included inside user applications and displays AR scene
 by implementing the QML interface to the QMLAR library
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-02
 */

import QtQuick 2.6
import AR 1.0
import QtQuick.Scene3D 2.0
import QtQuick.Controls 1.4 as QQC
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtMultimedia 5.9 as QMM

/** @brief This component is included inside user applications and displays AR scene
 * by implementing the QML interface to the QMLAR library.
 * See README.md for more examples

 * Usage:
 * 1. Set width
 * 2. Set arSceneComponent (loaded using Qt.createComponent) with arSceneParameters
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

    /** Set the image source, one of
     * AR.INIT_CAMERA -- use camera backend.
     * AR.INIT_IMAGE -- use fixed image (from FS) backend.
     * Default value is AR.INIT_CAMERA.
     * @see QMLAR InitTypes enum
     */
    property int init_type: AR.INIT_CAMERA

    /** ID of the camera to use. Set to -1 to use the default value.
     * Usually the builtin webcam is 0 and the USB cam is 1.
     * Default value is -1.
     */
    property int camera_id: -1

    /** Path to image to open in AR.INIT_IMAGE mode
     * Default value: assets/dots.sample.png
     * @see init_type
     */
    property string image_filename: "assets/dots.sample.png"

    /** Set the image width for image processing pipeline (CPU-based).
     * Must be set before creation, otherwise has no effect.
     * Default value: 640
     */
    property int image_width: 640

    /** A component containing a 3D scene (with .createObject() method)
     * @see arSceneParameters
     * @see arSceneObject
     */
    property var arSceneComponent

    /** Parameters to the arSceneComponent passed on creation
     * Default value: ({})
     * @see arSceneComponent
     */
    property var arSceneParameters: ({})

    /** The resulting object created by arSceneComponent.createObject() call
     * Useful for modifying scene properties at runtime.
     * @see arSceneComponent
     */
    property var arSceneObject

    /** (internal) The AR3DScene.qml object, containing camera and render settings */
    property var scene3d

    /** (internal) The AR3DScene.qml object, containing LEFT camera and render settings.
     * Used only in AVR mode
     * @see avr_mode
     */
    property var scene3d_left

    /** (internal) The AR3DScene.qml object, containing RIGHT camera and render settings.
     * Used only in AVR mode
     * @see avr_mode
     */
    property var scene3d_right

    /** Set to true to resize the component based on width property preserving aspect ration.
     * Valid only before startup, further changes have no effect.
     * If false, no size adjustments will be made.
     * Default value: true
     * @see width
     */
    property bool force_width: true

    /** Show FPS/Latency/Framedrop status at the top of the component?
     * Default value: true
     */
    property bool show_fps: true

    /** What to show on the component?
     * AR.OUTPUT_MARKERS -- display dots and markers from the detector.
     * AR.OUTPUT_CAMERA -- display raw camera image.
     * Has no effect on Android if camera is used, because it uses Viewfinder to output image.
     * Default value: AR.OUTPUT_CAMERA
     */
    property int output_type: AR.OUTPUT_CAMERA

    /** (internal) QQuickImageProvider url, used if viewfinder is unavailable. This image is updated using a Timer */
    property string output_url: ""

    /** (internal) The viewfinder object for Android */
    property var viewfinder

    /** (internal) The scaling coefficient for ARBlobs */
    property real scaleDots: 1.0

    /** Display detected blobs?
     * Default value: false */
    property bool show_blobs: false

    /** Display detected markers?
     * Default value: false */
    property bool show_markers: false

    /** Force using QImage+Timer instead of Viewfinder (debug option)
     * Default value: false
     */
    property bool force_image: false

    /** Enable Augmented Virtual Reality (AVR) mode, showing 2 cameras, for left and right eye
     * Default value: false */
    property bool avr_mode: false

    /** Disables the ability to enter the hidden menu by pressing at the center of the screen
     * Default value: false */
    property bool disable_menu: false

    /** An output which shows whether or not markers are visible right now */
    property bool markers_visible: AR.markers_visible

    /** Returns 4x4 ModelViewProjection matrix used for rendering */
    property var mvp_matrix: AR.mvp_matrix

    /** Initial width */
    width: 300

    /** Initial height */
    height: 300

    /** A signal which is called at each click on the AR component
     * @param x_mm The clicked point x coordinate (right) on the activity in mm
     * @param y_mm The clicked point y coordinate (down) on the activity in mm

     * Emitted only of markers are visible now.*/
    signal clickedOnActivity(real x_mm, real y_mm);

    /** A signal which is called when the arSceneComponent has finished loading
     * @see arSceneComponent
     * @see arSceneObject */
    signal scene_loaded();

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
        if(AR.camera !== null) {
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

    /** (internal) load the 3D scene */
    function load_scene3d() {
        console.log("Begin loading scene3d");
        var component = Qt.createComponent("AR3DScene.qml");
        if(component.status === Component.Error) {
            console.debug("Error loading scene: " + component.errorString());
        }
        else {
            if(avr_mode) {
                scene3d_left = component.createObject(scene, {'arSceneComponent': arSceneComponent,
                                           'arSceneParameters': arSceneParameters});
                scene3d_right = component.createObject(scene, {'arSceneComponent': arSceneComponent,
                                           'arSceneParameters': arSceneParameters});
                var eye_delta = 10; //(mm)
                scene3d_left.anchors.left = scene.left;
                scene3d_left.anchors.bottom = scene.bottom
                scene3d_left.anchors.top = scene.top;
                scene3d_left.width = 0.5 * scene.width;
                scene3d_left.delta_x = eye_delta;
                scene3d_right.anchors.left = scene3d_left.right;
                scene3d_right.anchors.bottom = scene.bottom
                scene3d_right.anchors.top = scene.top;
                scene3d_right.width = 0.5 * scene.width;
                scene3d_right.delta_x = -eye_delta;
            }
            else {
                scene3d = component.createObject(scene, {'arSceneComponent': arSceneComponent,
                                                     'arSceneParameters': arSceneParameters});
                scene3d.anchors.fill = scene;
                arSceneObject = scene3d.arSceneObject;
            }
            loading.visible = false;
        }
        console.log("End loading scene3d");
    }

    /** (internal) create viewfinder component */
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

        // Detect moust clicks inside to allow for drag&drop
        MouseArea {
            id: scene_mouse_area
            anchors.fill: parent
            onClicked: {
                // send it still to others
                // see https://stackoverflow.com/questions/16183408/mousearea-stole-qml-elements-mouse-events
                mouse.accepted = false;

                // checking if the pose is valid
                if(!AR.pose_valid) {
                    console.log('Clicked on AR component, but no pose available');
                    return;
                }

                // register the click
                var x_ndc = +2. * mouseX / width  - 1;
                var y_ndc = -2. * mouseY / height + 1;

                // MVP matrix from AR
                var mat = AR.mvp_matrix;

                // inverse transformation
                var world_xyz1 = Qt.matrix4x4(mat.m11, mat.m12, mat.m14, 0,
                                        mat.m21, mat.m22, mat.m24, 0,
                                        mat.m31, mat.m32, mat.m34, 0,
                                        0      , 0      , 0      , 1).inverted().times(Qt.vector4d(x_ndc, y_ndc, 1, 1));

                // outputtting
                var x_mm = world_xyz1.x / world_xyz1.z;
                var y_mm = world_xyz1.y / world_xyz1.z

                // debug output
                console.log("Clicked at " + x_mm + ", " + y_mm);

                // calling the signal
                clickedOnActivity(x_mm, y_mm);
            }
            z: 20
            propagateComposedEvents: true
        }

        // indicator showing loading circle
        QQC.BusyIndicator {
            z: 10
            id: loading
            running: true
            visible: true
            anchors.fill: parent
        }

        // Resize AR component on first valid image
        Timer {
            id: init_timer
            interval: 100; running: false; repeat: true;
            onTriggered: {
                var w = image.sourceSize.width;
                var h = image.sourceSize.height;
                if(avr_mode) {
                    w *= 2;

                    // can't use 2xVideoOutput for now...
                    force_image = true;
                }
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
                    if(AR.camera !== null && !root.force_image) {
                        console.log("Hiding image");
                        image.cache = 0;
                        image.source = "";
                        image_timer.running = false;
                        image.visible = false;
                    }
                    else {
                        console.log("Showing image");
                        if(avr_mode) {
                            image_avr_left.visible = true;
                            image_avr_right.visible = true;
                        }
                        else {
                            image.visible = true;
                        }
                        if(AR.camera !== null)
                            viewfinder.visible = 0;
                    }

                    // load scene on component loading
                    root.load_scene3d();
                    scene_loaded();
                }
            }
        }

        // hidden menu switch
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
                onClicked: if(!root.disable_menu) hiddenMenu.visible = !hiddenMenu.visible
            }
        }

        // the hidden menu
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

        // displaying blobs on request
        ARBlobs {
            id: blobs
            visible: root.show_blobs
            scaleDots: root.scaleDots
            z: 10
        }

        // displaying markers on request
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
            onTriggered: {
                if(avr_mode) {
                    image_avr_left.cache = 0;
                    image_avr_left.source = "";
                    image_avr_left.source = root.output_url;
                    image_avr_right.cache = 0;
                    image_avr_right.source = "";
                    image_avr_right.source = root.output_url;
                }
                image.cache = 0; image.source = ""; image.source = root.output_url;
            }
        }

        // image with camera image
        Image {
            id: image
            anchors.fill: parent
            visible: false
            clip: false
            transformOrigin: Item.Center
        }

        // AVR image with camera image
        Image {
            id: image_avr_left
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 0.5 * parent.width
            visible: false
            clip: false
            transformOrigin: Item.Center
        }

        // AVR image with camera image
        Image {
            id: image_avr_right
            anchors.left: image_avr_left.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 0.5 * parent.width
            visible: false
            clip: false
            transformOrigin: Item.Center
        }

        // text showing FPS and latency
        Text {
            z: 10
            id: fps_text
            color: "#4286f4"
            font.pointSize: 20

            visible: root.show_fps

            text: "FPS: " + Math.round(AR.FPSMean)
                  + " ± " + Math.round(AR.FPSStd) + " / "
                  + "Lat: " + Math.round(AR.LatencyMean)
                  + " ± " + Math.round(AR.LatencyStd) + " ms / "
                  + "F.Drop: " + Math.round(AR.framedrop * 100) + "% / "
                  + "F.Delay: " + AR.frame_delay
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
