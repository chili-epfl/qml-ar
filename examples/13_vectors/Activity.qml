/**
 * @file Activity.qml
 * @brief This file renders the scene in Qt3D
 * @author Sergei Volodin
 * @version 1.0
 * @date 2018-08-03
 */

import Qt3D.Core 2.0
import Qt3D.Extras 2.0

/** @brief a cube at (0, 0, 0) */
Entity {
    function clicked(x_mm, y_mm) {
        console.log("ABAA", x_mm, y_mm, demo.lst);

        var z_mm = 0;
        var closest_i = -1;
        var found = 0;

        var vec = Qt.vector3d(x_mm, y_mm, z_mm);
        var threshold = 10;

        for(var i = 0; i < demo.lst.length; i++)
        {
            var arrow = demo.lst[i];
            var from = arrow.lvector.from;
            var to = arrow.lvector.to;
            console.log(from.minus(vec).length());
            if(from.minus(vec).length() <= threshold)
            {
                console.log("Clicked at FROM", i);
            }
            if(to.minus(vec).length() <= threshold)
            {
                console.log("Clicked at TO", i);
            }
        }
    }

    DemoVectors {
        id: demo
    }
}
