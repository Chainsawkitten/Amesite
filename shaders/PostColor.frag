/*
Adjust color fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tExtra;
uniform sampler2D tDepth;

uniform vec3 color;

in vec2 texCoords;

out vec4 fragmentColor;

void main () {
    fragmentColor = texture(tDiffuse, texCoords) * vec4(color, 1.0);

    float depth = texture(tDepth, texCoords).r;
    gl_FragDepth = depth;
}