/*
Copy fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;

in vec2 texCoords;

out vec4 fragmentColor;

void main () {
	fragmentColor = texture(tDiffuse, texCoords);
	float depth = texture(tDepth, texCoords).r;
	gl_FragDepth = depth;
}