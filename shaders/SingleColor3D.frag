/*
Render 3D geometry with a single color.
*/
#version 400

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn;

uniform vec4 color;

out vec4 fragmentColor;

void main() {
    fragmentColor = color;
}
