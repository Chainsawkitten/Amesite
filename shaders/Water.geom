/*
Cull backfaces of incoming triangles and prepare values for Phong fragment shader.
*/
#version 400

// Input
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn[3];

// Uniforms.
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform mat4 projectionMatrix;
uniform vec4 clippingPlane;

// Output
out VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
    vec3 viewPosition;
} vertexOut;

void main() {
    for(int i = 0; i < gl_in.length(); i++) {
        // Copy attributes
        vertexOut.normal =  normalize(normalMatrix * vertexIn[i].normal);
        vertexOut.tangent = normalize(normalMatrix * vertexIn[i].tangent);
        vertexOut.texCoords = vertexIn[i].texCoords;
        vertexOut.viewPosition = vec3(viewMatrix * (modelMatrix * gl_in[i].gl_Position));
        gl_Position = projectionMatrix * vec4(vertexOut.viewPosition, 1.0);
    
        // Done with the vertex
        EmitVertex();
    }
    EndPrimitive();
}