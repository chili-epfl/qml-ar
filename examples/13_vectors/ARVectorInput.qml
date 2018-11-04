import QtQuick 2.0
import ch.epfl.chili.qmlar 1.0

// Records clicks on AR component, outputs the vector
Item {
    id: root

    // AR component
    property ARComponent arComponent

    // 0 -- waiting for start 1 -- waiting for 1st click 2 -- waiting for 2nd click
    property int status: 0

    // is active?
    property bool on: true

    // resulting vector
    property alias vector: lvec_public

    // emitted on an update
    signal updated();

    // resulting vector
    LocalizedVector {
        id: lvec_public
        from: Qt.vector3d(0, 0, 0)
        to: Qt.vector3d(0, 0, 0)
    }

    // resulting vector
    LocalizedVector {
        id: lvec
        from: Qt.vector3d(0, 0, 0)
        to: Qt.vector3d(0, 0, 0)
    }

    // begin the measurement
    function start() {
        if(root.status == 0)
            root.status = 1;
    }

    // reacting on click on the component
    Connections {
        target: arComponent
        enabled: root.on
        onClickedOnActivity: {
            if(root.status == 1) {
                lvec.from = Qt.vector3d(x_mm, y_mm, 0);
                root.status = 2;
            }
            else if(root.status == 2) {
                lvec.to = Qt.vector3d(x_mm, y_mm, 0);
                lvec_public.from = lvec.from;
                lvec_public.to = lvec.to;
                root.status = 0;
                updated();
            }
        }
    }
}
