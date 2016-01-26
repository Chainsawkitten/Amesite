/*
Simple pass-through vertex shader - Vertex Shader
*/
#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 textureCoordinate;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

void main () {
	gl_Position = vec4(vertexPosition, 1.0);
}