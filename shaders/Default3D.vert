/*
Simple pass-through vertex shader - Vertex Shader
*/
#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinate;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main () {
    gl_Position = projection*view*model*vec4(vertexPosition, 1.0);
}