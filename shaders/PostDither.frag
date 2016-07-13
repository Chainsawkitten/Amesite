/*
Copy fragment shader (using dithering).
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDepth;

uniform float time;

in vec2 texCoords;

out vec4 fragmentColor;

highp float rand(vec2 co) {
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt = dot(co.xy, vec2(a,b));
    highp float sn = mod(dt, 3.14);
    return fract(sin(sn) * c);
}

void main () {
    float dither = rand(texCoords + vec2(time, 0.0)) / 255.0;
    fragmentColor = texture(tDiffuse, texCoords) + vec4(dither);
    float depth = texture(tDepth, texCoords).r;
    gl_FragDepth = depth;
}
