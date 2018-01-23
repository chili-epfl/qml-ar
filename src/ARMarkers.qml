import QtQuick 2.0
import AR 1.0

Item {
    id: root
    anchors.fill: parent
    property real scaleDots: 1.0
    property real last_update: 0
    property real min_delta_ms: 1000
    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: doPaint()

        function doPaint() {
            if(!root.visible) return;
            var this_update = new Date().getTime();
            if(this_update - root.last_update < root.min_delta_ms) return;
            root.last_update = this_update;
            var markers = AR.markers;
            var ctx = getContext("2d");
            ctx.reset();
            ctx.fillStyle = "green";
            ctx.globalAlpha = 0.3;
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
            canvas.requestPaint()
        }
        Connections {
            target: AR
            onNewMarkers: canvas.doPaint()
        }
    }
}
