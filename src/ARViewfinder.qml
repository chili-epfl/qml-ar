import QtQuick 2.0
import QtMultimedia 5.4
import AR 1.0

// Viewfinder for Android instead of image+timer
VideoOutput {
    id: viewfinder
    visible: true
    anchors.fill: parent
    filters: [androidFilter]
    //autoOrientation: true

    AndroidFilter {
        id: androidFilter
    }
}
