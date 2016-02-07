/*
Particle geometry shader - Creates rectangles from points and faces them towards player
*/
#version 400

// Input
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in VertexData {
	float alpha;
	vec2 size;
	vec3 color;
	int textureIndex;
} vertexIn[1];

// Uniform matrices.
uniform mat4 viewProjectionMatrix;

// Uniform camera position
uniform vec3 cameraPosition;
uniform vec3 cameraUp;

// Uniform integer for number of atlas rows.
uniform int textureAtlasRows;

// Output
out VertexData {
	vec2 texCoords;
	float alpha;
	vec3 color;
} vertexOut;

void main() {
	vertexOut.color = vertexIn[0].color;
	
	int atlasColumn = int(mod(vertexIn[0].textureIndex, textureAtlasRows));
	int atlasRow = int(floor(vertexIn[0].textureIndex/textureAtlasRows));
	
	float atlasIndexOffset = 1.0/float(textureAtlasRows);
	vec2 atlasCoordOffset = vec2(float(atlasColumn/textureAtlasRows), float(atlasRow/textureAtlasRows));

	// Vector from camera to point, cameras position extracted from view matrix
	vec4 look = vec4(cameraPosition - gl_in[0].gl_Position.xyz, 0.0);

	// Particle right vector = camera up x look
	vec4 right = normalize(vec4(cross(cameraUp, look.xyz), 0.0)) * vertexIn[0].size.x * 0.5;
	
	// Particle up vector = particle right x look
	vec4 up = normalize(vec4(cross(look.xyz, right.xyz), 0.0)) * vertexIn[0].size.y * 0.5;
	
	gl_Position = viewProjectionMatrix * (gl_in[0].gl_Position - right + up);
	vertexOut.texCoords = atlasCoordOffset;
	vertexOut.alpha = vertexIn[0].alpha;
	EmitVertex();

	gl_Position = viewProjectionMatrix *  (gl_in[0].gl_Position - right - up);
	vertexOut.texCoords = vec2(atlasCoordOffset.x+atlasIndexOffset, atlasCoordOffset.y);
	vertexOut.alpha = vertexIn[0].alpha;
	EmitVertex();

	gl_Position = viewProjectionMatrix *  (gl_in[0].gl_Position + right + up);
	vertexOut.texCoords = vec2(atlasCoordOffset.x, atlasCoordOffset.y+atlasIndexOffset);
	vertexOut.alpha = vertexIn[0].alpha;
	EmitVertex();

	gl_Position = viewProjectionMatrix *  (gl_in[0].gl_Position + right - up);
	vertexOut.texCoords = vec2(atlasCoordOffset.x+atlasIndexOffset, atlasCoordOffset.y+atlasIndexOffset);
	vertexOut.alpha = vertexIn[0].alpha;
	EmitVertex();

	EndPrimitive();
}