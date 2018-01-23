import QtQuick 2.0
import AR 1.0

Item {
    id: root
    anchors.fill: parent
    property real scaleDots: 1.0
    property int last_update: new Date().getTime()
    property int min_delta_ms: 50
    Canvas {
        id: canvas
        anchors.fill: parent
        onPaint: doPaint()
        function doPaint() {
            if(!root.visible) return;
            if(new Date().getTime() - last_update < min_delta_ms) return;
            var blobs = AR.blobs;
            var ctx = getContext("2d");
            ctx.reset()
            ctx.fillStyle = "red";
            ctx.globalAlpha = 0.2;
            for(var i = 0; i < blobs.length; i++) {
                ctx.beginPath();
                var blob = blobs[i];
                ctx.arc(blob.x * root.scaleDots, blob.y * root.scaleDots, 10, 0, Math.PI * 2, false);
                ctx.fill();
            }
            canvas.requestPaint()
        }
        Connections {
            target: AR
            onNewBlobs: canvas.doPaint()
        }
    }
}
