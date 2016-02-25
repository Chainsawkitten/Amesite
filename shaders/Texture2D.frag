/*
Render a texture to the screen.
*/
#version 400

in VertexData {
	vec2 texCoords;
} vertexIn;

uniform sampler2D baseImage;
uniform float alpha;

out vec4 fragmentColor;

void main () {
	vec4 sampled = texture(baseImage, vertexIn.texCoords);
    fragmentColor = vec4(sampled.rgb, sampled.a * alpha);
}