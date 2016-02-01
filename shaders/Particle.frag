#version 400
in VertexData {
	vec2 texCoords;
	float alpha;
} vertexIn;

uniform sampler2D baseImage;
uniform vec3 color;

out vec4 fragment_color;
void main () {
	fragment_color = texture(baseImage, vertexIn.texCoords);
	fragment_color.rgb *= color;
	fragment_color.a *= vertexIn.alpha;
}