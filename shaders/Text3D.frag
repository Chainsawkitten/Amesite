/*
Render 3D text (forward rendering)
*/
#version 400

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn;

uniform sampler2D baseImage;

uniform vec3 color;

out vec4 fragmentColor;

void main() {
    fragmentColor = vec4(color, texture(baseImage, vertexIn.texCoords).a);
    
    if (fragmentColor.a < 0.05)
        discard;
}
