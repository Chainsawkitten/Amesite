/*
Cel shading fragment shader.
Creates lit image from intermediate results.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tDiffuseLevels;
uniform sampler2D tSpecular;
uniform sampler2D tGlow;
uniform sampler2D tDepth;

uniform float steps = 20.0;

in vec2 texCoords;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

vec3 stepify(vec3 color) {
    return floor(color * steps) / (steps - 1.0);
}

void main () {
    float depth = texture(tDepth, texCoords).r;
    vec3 diffuse = texture(tDiffuse, texCoords).rgb;
    vec3 diffuseLevel = texture(tDiffuseLevels, texCoords).rgb;
    vec3 specular = texture(tSpecular, texCoords).rgb;
    
    fragmentColor = vec4(diffuse * stepify(diffuseLevel) + specular, 1.0);
    extraOut = vec4(texture(tGlow, texCoords).rgb, 1.0);
    gl_FragDepth = depth;
}