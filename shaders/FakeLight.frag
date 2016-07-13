/*
Fake point light fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tGlow;

uniform vec3 lightColor;
uniform vec2 screenSize;

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

void main() {
    vec2 screenCoords = gl_FragCoord.xy / screenSize;
    vec3 diffuse = texture(tDiffuse, screenCoords).rgb;
    vec2 dis = vertexIn.texCoords - vec2(0.5, 0.5);
    float attenuation = clamp(1.004 / (1.0 + 1000.0 * (dis.x * dis.x + dis.y * dis.y)) - 0.004, 0.0, 1.0);
    fragmentColor = vec4(attenuation * diffuse * lightColor * 0.12, 1.0);
    extraOut = texture(tGlow, screenCoords);
}
