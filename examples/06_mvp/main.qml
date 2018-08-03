/**
 * @file main.qml
 * @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it show how to obtain the raw MVP matrix from AR lib.
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import QtQuick.Window 2.2
import QtQuick 2.6
import ch.epfl.chili.qmlar 1.0

/** @brief This project renders a cube at (0, 0, 0) using qml-ar.
 * Moreover, it show how to obtain the raw MVP matrix from AR lib. */
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
    }

    // showing a text in the middle with matrix components
    Text {
        anchors.centerIn: parent
        font.pixelSize: 30
        text: pretty_print_matrix4(arComponent.mvp_matrix)
        color: "red"
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
