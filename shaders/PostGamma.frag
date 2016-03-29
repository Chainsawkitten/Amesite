/*
Gamma correction.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;

uniform float brightness;

in vec2 texCoords;

out vec4 fragmentColor;

void main () {
    vec3 color = texture(tDiffuse, texCoords).rgb;
    vec3 gamma = vec3(1.0 / brightness);
    fragmentColor = vec4(pow(color, gamma), 1.0);
    float depth = texture(tDepth, texCoords).r;
    gl_FragDepth = depth;
}