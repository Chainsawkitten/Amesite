/*
Lighting pass fragment shader (second pass)
Directional light (sunlight).
*/
#version 400

uniform sampler2D tShadowMap;
uniform sampler2D tDiffuse;
uniform sampler2D tNormals;
uniform sampler2D tSpecular;
uniform sampler2D tGlow;
uniform sampler2D tDepth;

uniform mat4 inverseProjectionMatrix;

uniform struct Light {
    vec4 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;
    vec3 direction;
} light;

uniform float scale;

in vec2 texCoords;

layout(location = 0) out vec4 diffuseOut;
layout(location = 1) out vec4 specularOut;

// Apply ambient, diffuse and specular lighting.
void ApplyLight(vec3 surfaceColor, vec3 normal, vec3 position, vec3 surfaceSpecular) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    
    if (light.position.w == 0.0) {
        // Directional light.
        surfaceToLight = normalize(light.position.xyz);
        attenuation = 1.0;
    } else {
        // Point light.
        vec3 toLight = light.position.xyz - position;
        surfaceToLight = normalize(toLight);
        attenuation = 1.0 / (1.0 + light.attenuation * (toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z));
        
        // Spot light.
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.direction))));
        if (lightToSurfaceAngle > light.coneAngle)
            attenuation = 0.0;
    }
    
    // Diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    
    // Specular
    vec3 surfaceToCamera = normalize(-position);
    float materialShininess = 20.0;
    float specularCoefficient = 0.0;
    if (diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(dot(surfaceToCamera, reflect(-surfaceToLight, normal)), 0.0), materialShininess);
    
    // Linear color (color before gamma correction)
    diffuseOut = vec4(light.ambientCoefficient + attenuation * diffuseCoefficient * light.intensities, 1.0);
    specularOut = vec4(attenuation * specularCoefficient * surfaceSpecular * light.intensities, 1.0);
}

// Reconstruct position.
vec3 reconstructPos(vec2 texCoord, float depth){
    vec4 sPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    sPos = inverseProjectionMatrix * sPos;

    return (sPos.xyz / sPos.w);
}

void main () {
    float depth = texture(tDepth, texCoords * scale).r;
    vec3 position = reconstructPos(texCoords, depth);
    vec3 diffuse = texture(tDiffuse, texCoords * scale).rgb;
    vec3 normal = texture(tNormals, texCoords * scale).xyz;
    vec3 specular = texture(tSpecular, texCoords * scale).xyz;
    
    ApplyLight(diffuse, normalize(normal), position, specular);
    gl_FragDepth = depth;
}