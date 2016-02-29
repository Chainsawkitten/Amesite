/*
Lighting pass vertex shader (second pass)
*/
#version 400

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexTexture;

out vec2 texCoords;

void main () {
    texCoords = vertexTexture;
    gl_Position = vec4(vertexPosition, 0.0, 1.0);
}