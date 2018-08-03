/**
 * @file ARBlobs.qml
 * @brief This component shows blobs detected by the AR library
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-02
 */

import QtQuick 2.0
import ch.epfl.chili.qmlar 1.0

/**
 * @brief This component shows blobs detected by the AR library
 */
Item {
    id: root
    anchors.fill: parent

    /** The scaling coefficient for ARBlobs */
    property real scaleDots: 1.0

    // canvas for painting blobs
    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: doPaint()

        // paint blobs
        function doPaint() {
            if(!root.visible) return;

            // setting up
            var blobs = AR.blobs;
            var ctx = getContext("2d");
            ctx.reset()
            ctx.fillStyle = "blue";
            ctx.globalAlpha = 0.3;

            // plotting circles
            for(var i = 0; i < blobs.length; i++) {
                ctx.beginPath();
                var blob = blobs[i];
                ctx.arc(blob.x * root.scaleDots, blob.y * root.scaleDots, 10, 0, Math.PI * 2, false);
                ctx.fill();
            }

            // finishing painting
            canvas.requestPaint()
        }

        // auto repaint
        Connections {
            target: AR
            onNewBlobs: canvas.doPaint()
        }
    }
}
