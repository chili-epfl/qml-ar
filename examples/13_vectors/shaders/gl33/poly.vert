#version 330 core
uniform mat4 mvp;
uniform mat4 modelMatrix;

in vec3 vertexPosition;

out vec3 worldPosition;

void main() {
    // Transform position, normal, and tangent to world coords

    worldPosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
    gl_Position = mvp * vec4(vertexPosition, 1.0f);
}
