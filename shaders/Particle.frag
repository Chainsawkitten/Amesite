#version 400
in VertexData {
	vec2 texCoords;
	float alpha;
	vec3 color;
} vertexIn;

uniform sampler2D baseImage;

out vec4 fragment_color;
void main () {
	fragment_color = texture(baseImage, vertexIn.texCoords);
	fragment_color.rgb *= vertexIn.color;
	fragment_color.a *= vertexIn.alpha;
}