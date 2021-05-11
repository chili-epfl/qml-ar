/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it shows the pose of the device w.r.t. the markers.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick 2.6
import Qt3D.Core 2.0
import ch.epfl.chili.qmlar 1.0

/** @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it shows the pose of the device w.r.t. the markers. */
Window {
    // some window parameters
    visible: true
    height: 500
    width: 500

    // crearing AR component
    ARComponent {
        id: arComponent

        // loading scene
        arSceneComponent: Qt.createComponent("Activity.qml");

        // setting width
        width: 800

//        init_type: AR.INIT_IMAGE
    }

    // showing a text in the middle with matrix components
    Text {
        anchors.centerIn: parent
        font.pixelSize: 30
        text: pretty_print_transform()
        color: "red"
    }

    function get_camera_matrix() {
        // Get the calibrated camera matrix.
        // TODO: read the asset instead of using the hardcoded matrix

        // from camera_matrix.json
        var C = [
                    7.3321993364036177e+02, 0., 2.9429448615773646e+02,
                    0., 7.3321993364036177e+02, 6.8074088355361582e+01,
                    0., 0., 1.
                ];

        return C;
    }

    function get_projection_matrix_manual(H, W) {
        // from camera_matrix.json
        var Hcalib = 480;
        var Wcalib = 640;

        // from markermvpprovider.h
        var n = 1;
        var f = 10000;

//        console.log('image h=', H, 'W=', W);

        // Get the projection matrix manually.
        // mimicks markermvpprovider.cpp
        // translate (-1, 1, 0)
        var translate_m1 = Qt.matrix4x4(1, 0, 0, -1,
                                        0, 1, 0, 1,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);

        // scale to image
        var scaler = Qt.matrix4x4(2. / W, 0, 0, 0,
                                  0, -2. / H, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1)

        // camera matrix (unscaled) from camera_matrix.json
        var C = get_camera_matrix();

        // projection matrix, see calibratedcamera.cpp
        var project = Qt.matrix4x4(
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0);

        // multiply the camera matrix by this, see
        // calibratedcamera.cpp -> getScaler
        var scaler_camera = W / Wcalib;

//        console.log("qml scaler", scaler_camera);

        // copying the matrix
        project.m11 = C[0] * scaler_camera;
        project.m12 = C[1] * scaler_camera;
        project.m13 = C[2] * scaler_camera;

        project.m21 = C[3] * scaler_camera;
        project.m22 = C[4] * scaler_camera;
        project.m23 = C[5] * scaler_camera;

        project.m31 = C[6] * scaler_camera;
        project.m32 = C[7] * scaler_camera;
        project.m33 = C[8] * scaler_camera;

        // see perspectivecamera.cpp
        project.m33 = 0;
        project.m43 = 1;

        project.m33 = (f + n) / (f - n);
        project.m34 = -2 * n * f / (f - n);

//        console.log("p_matrix qml project", project);

        // total, see markermvpprovider.cpp
        var p_matrix = translate_m1.times(scaler.times(project));

//        console.log('p_matrix qml', p_matrix);

        return p_matrix;
    }

    // get ModelView matrix from the MVP and P matrices
    function get_mv(mvp) {
        // todo: get the resolution from the lib instead of hand-coding
        // resolution taken from camera-> width/height (markermvpprovider.cpp)
        var p = get_projection_matrix_manual(270, 480);
        var mv = p.inverted().times(mvp);
        return mv;
    }

    // print the transform as rotation and translation
    function pretty_print_transform() {
        var res = "";
        var mv = get_mv(arComponent.mvp_matrix);

        // adapted from https://stackoverflow.com/questions/54616049/converting-a-rotation-matrix-to-euler-angles-and-back-special-case
        var beta = -Math.asin(mv.m31);
        var alpha = Math.atan2(mv.m32 / Math.cos(beta), mv.m33 / Math.cos(beta));
        var gamma = Math.atan2(mv.m21 / Math.cos(beta), mv.m11 / Math.cos(beta));

        beta = beta * 180 / Math.PI;
        alpha = alpha * 180 / Math.PI;
        gamma = gamma * 180 / Math.PI;

        res += 'b='  + Math.round(beta * 1) / 1 + '\t';
        res += 'a=' + Math.round(alpha * 1) / 1 + '\t';
        res += 'g=' + Math.round(gamma * 1) / 1 + '\t';

        var rot_mat = Qt.matrix4x4(mv.m11, mv.m12, mv.m13, 0,
                                   mv.m21, mv.m22, mv.m23, 0,
                                   mv.m31, mv.m32, mv.m33, 0,
                                   0     , 0     , 0     , 0);

        // translation
        var t = Qt.vector4d(mv.m14, mv.m24, mv.m34, 0);
        t = rot_mat.transposed().times(t);

        res += '\n';
        res += "x=" + Math.round(-t.x * 1) / 1 + '\t';
        res += "y=" + Math.round(-t.y * 1) / 1 + '\t';
        res += "z=" + Math.round(-t.z * 1) / 1 + '\t';

        return res;
    }
}
