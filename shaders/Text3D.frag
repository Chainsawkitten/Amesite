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

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(baseImage, vertexIn.texCoords);
    
    if (fragmentColor.a < 0.05)
        discard;
}
