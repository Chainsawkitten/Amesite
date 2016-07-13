/*
Copy fragment shader (using dithering).
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;
uniform sampler2D tDither;

in vec2 texCoords;

out vec4 fragmentColor;

void main () {
    float dither = texture(tDither, gl_FragCoord.xy * 0.125).r * 0.03125 - 0.0078125;
    fragmentColor = texture(tDiffuse, texCoords) + vec4(dither);
    float depth = texture(tDepth, texCoords).r;
    gl_FragDepth = depth;
}
