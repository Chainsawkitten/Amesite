/*
First pass glow filter. Blurs along the x-axis.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tExtra;
uniform sampler2D tDepth;

uniform vec3 color;

in vec2 texCoords;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

void main () {
	fragmentColor = texture(tDiffuse, texCoords);
	extraOut = texture(tExtra, texCoords);

	float depth = texture(tDepth, texCoords).r;
	gl_FragDepth = depth;
}