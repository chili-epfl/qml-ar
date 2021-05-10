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
        width: 500

        init_type: AR.INIT_IMAGE
    }

    // showing a text in the middle with matrix components
    Text {
        anchors.centerIn: parent
        font.pixelSize: 30
        text: pretty_print_matrix4(get_mv(arComponent.mvp_matrix)) + pretty_print_transform()
        color: "red"
    }

    function get_camera_matrix() {
        // Get the calibrated camera matrix.
        // TODO: read the asset instead of using the hardcoded matrix

        var C = [
                    7.3321993364036177e+02, 0., 2.9429448615773646e+02,
                    0., 7.3321993364036177e+02, 6.8074088355361582e+01,
                    0., 0., 1.
                ];

        return C;
    }

    function get_projection_matrix_manual(H, W) {
        var Hcalib = 480;
        var Wcalib = 640;
        var n = 1;
        var f = 10000;

        // Get the projection matrix manually.

        // translate (-1, 1, 0)
        var translate_m1 = Qt.matrix4x4(1, 0, 0, -1,
                                        0, 1, 0, 1,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1);

        var scaler = Qt.matrix4x4(2. / W, 0, 0, 0,
                                  0, -2. / H, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 1)

        var C = get_camera_matrix();

        var project = Qt.matrix4x4(
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0);

        project.m11 = C[0];
        project.m12 = C[1];
        project.m13 = C[2];

        project.m21 = C[3];
        project.m22 = C[4];
        project.m23 = C[5];

        project.m31 = C[6];
        project.m32 = C[7];
        project.m33 = C[8];

        project.m33 = 0;
        project.m43 = 1;

        project.m33 = (f + n) / (f - n);
        project.m34 = -2 * n * f / (f - n);

        return translate_m1.times(scaler.times(project));
    }

    function get_mv(mvp) {
        var p = get_projection_matrix_manual(arComponent.height, arComponent.width);
        var mv = p.inverted().times(mvp);
        return mv;
    }

    Matrix4x4 {
        id: mv_transform
        matrix: get_mv()
    }

    // print the transform as rotation and translation
    function pretty_print_transform() {
        var res = "";

        console.log(mv_transform.rotationX)

        var mv = get_mv(arComponent.mvp_matrix);

        // translation
        var t = Qt.vector3d(mv.m14, mv.m24, mv.m34);

        res += "x=" + Math.round(t.x * 1) / 1 + '\t';
        res += "y=" + Math.round(t.y * 1) / 1 + '\t';
        res += "tz=" + Math.round(t.z * 1) / 1 + '\t';
        res += '\n';


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

        return res;
    }

    /** Print matrix 4x4 in a nice way */
    function pretty_print_matrix4(mat) {
        var res = "";
        var arr = [mat.m11, mat.m12, mat.m13, mat.m14,
                   mat.m21, mat.m22, mat.m23, mat.m24,
                   mat.m31, mat.m32, mat.m33, mat.m34,
                   mat.m41, mat.m42, mat.m43, mat.m44]
        for(var i = 0; i < 4; i++) {
            for(var j = 0; j < 4; j++) {
                var index = i * 4 + j;
                res += Math.round(arr[index] * 100) / 100 + '\t';
            }
            res += '\n';
        }
        return res;
    }
}
