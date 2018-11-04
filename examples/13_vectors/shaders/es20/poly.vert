#define FP highp

uniform FP mat4 mvp;
uniform FP mat4 modelMatrix;
attribute vec3 vertexPosition;
varying FP vec3 worldPosition;

void main() {
    worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
