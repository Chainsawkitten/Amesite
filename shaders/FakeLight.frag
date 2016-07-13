/*
Fake point light fragment shader.
*/
#version 400

uniform sampler2D tDiffuse;

uniform vec3 lightColor;
uniform vec2 screenSize;

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
} vertexIn;

out vec4 fragmentColor;

void main() {
    vec2 screenCoords = gl_FragCoord.xy / screenSize;
    vec3 diffuse = texture(tDiffuse, screenCoords).rgb;
    dvec2 dis = vertexIn.texCoords - vec2(0.5, 0.5);
    double attenuation = 1.0 / (1.0 + 1773.778 * (dis.x * dis.x + dis.y * dis.y));
    fragmentColor = vec4(attenuation * diffuse * lightColor * 0.12, 1.0);
}
