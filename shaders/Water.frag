/*
Water fragment shader (reflection and refraction).
*/
#version 400

in VertexData {
    vec3 normal;
    vec3 tangent;
    vec2 texCoords;
    vec3 viewPosition;
} vertexIn;

uniform sampler2D tRefraction;
uniform sampler2D tReflection;
uniform sampler2D tDuDvMap;
uniform sampler2D tWater;
uniform sampler2D tNormalMap;
uniform sampler2D tDepthMap;

uniform vec2 screenSize;
uniform vec2 textureRepeat;
uniform vec2 texOffset;
uniform float moveFactor;
uniform float zNear;
uniform float zFar;
const float waveStrength = 0.005;

uniform vec4 lightPosition;
uniform vec3 lightIntensity;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

// Calculate texcoord
vec2 CalculateTexCoord() {
    return gl_FragCoord.xy / screenSize;
}

vec3 CalculateNormal(in vec3 normal, in vec3 tangent, in vec3 mapNormal) {
    vec3 n = normalize(normal);
    vec3 t = normalize(tangent);
    t = normalize(t - dot(t, n) * n);
    vec3 b = cross(t, n);
    if (dot(cross(n, t), b) < 0.0)
        t = -t;
    
    vec3 mn = normalize(2.0 * mapNormal - vec3(1.0, 1.0, 1.0));
    mat3 TBN = mat3(t, b, n);
    return TBN * mn;
}

void main() {
    vec2 refractionTexCoord = CalculateTexCoord();
    vec2 reflectionTexCoord = refractionTexCoord;
    reflectionTexCoord.y = 1.0 - reflectionTexCoord.y;
    
    // Calculate depth of water.
    float depth = texture(tDepthMap, refractionTexCoord).r;
    float floorDistance = 2.0 * zNear * zFar / (zFar + zNear - (2.0 * depth - 1.0) * (zFar - zNear));
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * zNear * zFar / (zFar + zNear - (2.0 * depth - 1.0) * (zFar - zNear));
    depth = floorDistance - waterDistance;
    
    // Distortion.
    vec2 texCoords = texture(tDuDvMap, vertexIn.texCoords * textureRepeat + vec2(moveFactor, 0.f)).rg * 0.1;
    texCoords += vertexIn.texCoords * textureRepeat + texOffset + vec2(0.f, moveFactor);
    vec2 distortion1 = (texture(tDuDvMap, texCoords).rg * 2.0 - 1.0) * waveStrength * clamp(depth * 0.5, 0.0, 1.0);
    
    refractionTexCoord += distortion1;
    reflectionTexCoord += distortion1;
    
    vec4 refractionColor = texture(tRefraction, refractionTexCoord);
    vec4 reflectionColor = texture(tReflection, reflectionTexCoord);
    
    vec3 normal = CalculateNormal(vertexIn.normal, vertexIn.tangent, texture(tNormalMap, texCoords).xyz);
    
    // Fresnel effect.
    float refractiveFactor = sqrt(dot(normalize(-vertexIn.viewPosition), normalize(vertexIn.normal)));
    refractiveFactor = clamp(refractiveFactor, 0.0, 1.0);
    fragmentColor = mix(reflectionColor, refractionColor, refractiveFactor);
    fragmentColor = fragmentColor * mix(texture(tWater, texCoords), vec4(1.0, 1.0, 1.0, 1.0), 0.5);
    
    // Specular.
    /*float shinyPower = 20.0;
    vec3 v = normalize(-vertexIn.viewPosition);
    vec3 lightDirection = normalize(vec3(lightPosition) - vertexIn.viewPosition);
    vec3 r = normalize(reflect(-lightDirection, normal));
    vec3 specularLight = vec3(1.0, 1.0, 1.0) * pow(max(dot(r, v), 0.0), shinyPower) * clamp(depth, 0.0, 1.0);
    fragmentColor = vec4(lightIntensity * (fragmentColor.rgb + specularLight), 1.0);*/
    fragmentColor.a = clamp(depth, 0.0, 1.0);
    
    extraOut = vec4(0.0, 0.0, 0.0, 1.0);
}
