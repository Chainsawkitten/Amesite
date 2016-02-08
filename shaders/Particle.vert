/*
Passthrough vertex shader
*/
#version 400
layout(location = 0) in vec3 particlePosition;
layout(location = 1) in vec2 particleSize;
layout(location = 2) in float particleLife;
layout(location = 3) in float particleLifetime;
layout(location = 4) in vec3 particleVelocity;

uniform float alpha[3];

out VertexData {
	float alpha;
	vec2 size;
} vertexOut;

void main () {
	float a = 2.0 * particleLife / particleLifetime;
	vertexOut.alpha = (1.0 - fract(a)) * alpha[int(a)] + fract(a) * alpha[int(a)+1];
	vertexOut.size = particleSize;
	gl_Position = vec4(particlePosition + particleVelocity * particleLife, 1.0);
}