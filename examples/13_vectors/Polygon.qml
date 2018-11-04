import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0
import QtQuick 2.9

// this item displays a 3D polygon with custom software-defined vertices
Entity {
    id: root

    // array of vertices. format: [[x1, y1, z1], [x2, y2, z2], ... , [xn, yn, zn]]
    //property var vertices: [[100, 100, 0], [200, 100, 0], [200, 200, 0], [100, 200, 0]]
    property var vertices: []

    // flattened array of vertices
    property var verticesFlatten: []

    // PolygonVertices object
    property var verticesObject: null

    // update vertices on GPU
    function update() {
        if(root.verticesObject !== null) {
            root.verticesObject.destroy();
        }

        root.verticesFlatten = [];

        for(var v_id in root.vertices) {
            var vertex = root.vertices[v_id];
            for(var c_id in vertex) {
                root.verticesFlatten.push(1.0 * vertex[c_id])
            }
        }
        var verticesComponent = Qt.createComponent('PolygonVertices.qml');

        verticesObject = verticesComponent.createObject(root, {'points': root.verticesFlatten});
        root.components = [verticesObject, material];
    }

    // updating on change
    onVerticesChanged: update()

    // code partially taken from
    // https://github.com/MidoriYakumo/learnopengl-qt3d/blob/master/qml/hello-triangle-exercise2.qml

    // rendering the polygon
    Material {
        id: material

        effect: Effect {

            FilterKey {
                id: forward
                name: "renderingStyle"
                value: "forward"
            }

            ShaderProgram {
                id: gl3Shader
                vertexShaderCode: loadSource('qrc:/shaders/gl33/poly.vert')
                fragmentShaderCode: loadSource('qrc:/shaders/gl33/poly.frag')
            }
            ShaderProgram {
                id: esShader
                vertexShaderCode: loadSource('qrc:/shaders/es20/poly.vert')
                fragmentShaderCode: loadSource('qrc:/shaders/es20/poly.frag')
            }

            AlphaCoverage { id: alphaCoverage }

            DepthTest {
                id: depth_test
                depthFunction: DepthTest.Always
            }

            // critical, might not work otherwise
            CullFace {
                id: cull
                mode: CullFace.NoCulling
            }

            techniques: [
                // OpenGL 3.1
                Technique {
                    filterKeys: [ forward ]
                    graphicsApiFilter {
                        api: GraphicsApiFilter.OpenGL
                        profile: GraphicsApiFilter.CoreProfile
                        majorVersion: 3
                        minorVersion: 1
                    }
                    renderPasses: RenderPass {
                        shaderProgram: gl3Shader
                        renderStates: [ alphaCoverage, cull ]
                    }
                },
                // OpenGLES 2.0
                Technique {
                    filterKeys: [ forward ]
                    graphicsApiFilter {
                        api: GraphicsApiFilter.OpenGLES
                        majorVersion: 2
                        minorVersion: 0
                    }
                    renderPasses: RenderPass {
                        shaderProgram: esShader
                        renderStates: [ alphaCoverage, cull ]
                    }
                },

                // OpenGL ES 2
                Technique {
                    filterKeys: [ forward ]
                    graphicsApiFilter {
                        api: GraphicsApiFilter.OpenGLES
                        profile: GraphicsApiFilter.NoProfile
                        majorVersion: 2
                        minorVersion: 0
                    }
                    renderPasses: RenderPass {
                        shaderProgram: esShader
                        renderStates: [ alphaCoverage, cull ]
                    }
                }
            ]
        }
    }
}
