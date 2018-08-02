/**
 * @file ARMarkers.qml
 * @brief This component shows markers detected by the AR library
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-02
 */

import QtQuick 2.0
import AR 1.0

/**
 * @brief This component shows markers detected by the AR library
 */
Item {
    id: root
    anchors.fill: parent

    /** The scaling coefficient for ARBlobs */
    property real scaleDots: 1.0

    // canvas for painting markers
    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: doPaint()

        // paint markers
        function doPaint() {
            if(!root.visible) return;

            // setting up
            var markers = AR.markers;
            var ctx = getContext("2d");
            ctx.reset();
            ctx.fillStyle = "green";
            ctx.globalAlpha = 0.3;

            // plotting green transparent rectangles for markers
            for(var i = 0; i < markers.length; i += 4) {
                var marker_tl = markers[i];
                var marker_br = markers[i + 1];
                ctx.beginPath();
                ctx.moveTo(markers[i + 0].x * root.scaleDots, markers[i + 0].y * root.scaleDots);

                ctx.lineTo(markers[i + 1].x * root.scaleDots, markers[i + 1].y * root.scaleDots);
                ctx.lineTo(markers[i + 2].x * root.scaleDots, markers[i + 2].y * root.scaleDots);
                ctx.lineTo(markers[i + 3].x * root.scaleDots, markers[i + 3].y * root.scaleDots);
                ctx.lineTo(markers[i + 0].x * root.scaleDots, markers[i + 0].y * root.scaleDots);
                ctx.fill();
            }

            // finishing painting
            canvas.requestPaint()
        }

        // auto update
        Connections {
            target: AR
            onNewMarkers: canvas.doPaint()
        }
    }
}
