import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick 2.9

// Vertices of the polygon stored on GPU
GeometryRenderer {
    id: root

    // flattened array of points
    property var points: []

    // type of display
    primitiveType: GeometryRenderer.LineLoop

    // array with points on GPU
    geometry: Geometry {
        Attribute {
            attributeType: Attribute.VertexAttribute
            vertexBaseType: Attribute.Float
            vertexSize: 3
            count: root.points.length / 3
            name: "vertexPosition"
            buffer: Buffer {
                type: Buffer.VertexBuffer
                data: new Float32Array(root.points)
            }
        }
    }
}
