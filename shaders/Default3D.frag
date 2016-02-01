/*
Geometry pass fragment shader (first pass)
*/
#version 400

layout(location = 0) out vec3 diffuseOut;
layout(location = 1) out vec3 normalsOut;
layout(location = 2) out vec3 specularOut;

void main() {
	diffuseOut = vec3(1.0, 0.0, 0.0);
	normalsOut = vec3(0.5, 0.5, 0.0);
	specularOut = vec3(1.0, 1.0, 1.0);
}
