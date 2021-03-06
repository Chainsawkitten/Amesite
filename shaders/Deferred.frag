/*
Lighting pass fragment shader (second pass)
Directional light (sunlight).
*/
#version 400

uniform sampler2D tDiffuse;
uniform sampler2D tNormals;
uniform sampler2D tSpecular;
uniform sampler2D tGlow;
uniform sampler2D tDepth;

uniform mat4 inverseProjectionMatrix;

struct Light {
    vec4 position;
    vec3 intensities;
    float attenuation;
    vec3 direction;
    float ambientCoefficient;
    float coneAngle;
    float distance;
    float padding1;
    float padding2;
};

const int maxLightCount = 128;
layout (std140) uniform light_data {
    Light lights[maxLightCount];
};
uniform int lightCount;

in vec2 texCoords;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

// Apply ambient, diffuse and specular lighting.
vec3 ApplyLight(vec3 surfaceColor, vec3 normal, vec3 position, vec3 surfaceSpecular, int light) {
    vec3 surfaceToLight;
    float attenuation = 1.0;
    
    if (lights[light].position.w == 0.0) {
        // Directional light.
        surfaceToLight = normalize(lights[light].position.xyz);
        attenuation = 1.0;
    } else {
        // Point light.
        vec3 toLight = lights[light].position.xyz - position;
        surfaceToLight = normalize(toLight);
        float lightDist = toLight.x * toLight.x + toLight.y * toLight.y + toLight.z * toLight.z;
        attenuation = 1.0 / (1.0 + lights[light].attenuation * lightDist);
        
        // Fade-out close to cutoff distance.
        lightDist = sqrt(lightDist) / lights[light].distance;
        const float curveTransition = 0.7;
        const float k = 1.0 / (1.0 - curveTransition);
        attenuation *= clamp(k - k * lightDist, 0.0, 1.0);
        
        // Spot light.
        float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(lights[light].direction))));
        if (lightToSurfaceAngle > lights[light].coneAngle)
            attenuation = 0.0;
    }
    
    // Ambient.
    vec3 ambient = lights[light].ambientCoefficient * surfaceColor;
    
    // Diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor * lights[light].intensities;
    
    // Specular
    vec3 surfaceToCamera = normalize(-position);
    float materialShininess = 20.0;
    float specularCoefficient = 0.0;
    if (diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(dot(surfaceToCamera, reflect(-surfaceToLight, normal)), 0.0), materialShininess);
    vec3 specular = specularCoefficient * surfaceSpecular * lights[light].intensities;
    
    // Linear color (color before gamma correction)
    return ambient + attenuation * (diffuse + specular);
}

// Reconstruct position.
vec3 ReconstructPos(vec2 texCoord, float depth){
    vec4 sPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    sPos = inverseProjectionMatrix * sPos;

    return (sPos.xyz / sPos.w);
}

void main () {
    float depth = texture(tDepth, texCoords).r;
    vec3 position = ReconstructPos(texCoords, depth);
    vec3 diffuse = texture(tDiffuse, texCoords).rgb;
    vec3 normal = normalize(texture(tNormals, texCoords).xyz);
    vec3 specular = texture(tSpecular, texCoords).xyz;
    
    vec3 accumulatedLight;
    for (int i=0; i < lightCount; ++i) {
        accumulatedLight += ApplyLight(diffuse, normal, position, specular, i);
    }
    
    fragmentColor = vec4(accumulatedLight, 1.0);
    extraOut = vec4(texture(tGlow, texCoords).rgb, 1.0);
    gl_FragDepth = depth;
}