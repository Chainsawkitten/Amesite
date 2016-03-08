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
uniform vec2 screenSize;

uniform struct Light {
    vec4 position;
    vec3 intensities;
    float attenuation;
    float ambientCoefficient;
    float coneAngle;
    vec3 direction;
} light;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 extraOut;

// Apply ambient, diffuse and specular lighting.
vec3 ApplyLight(vec3 surfaceColor, vec3 normal, vec3 position, vec3 surfaceSpecular) {
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
    
    // Ambient.
    vec3 ambient = light.ambientCoefficient * surfaceColor;
    
    // Diffuse
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuse = diffuseCoefficient * surfaceColor * light.intensities;
    
    // Specular
    vec3 surfaceToCamera = normalize(-position);
    float materialShininess = 20.0;
    float specularCoefficient = 0.0;
    if (diffuseCoefficient > 0.0)
        specularCoefficient = pow(max(dot(surfaceToCamera, reflect(-surfaceToLight, normal)), 0.0), materialShininess);
    vec3 specular = specularCoefficient * surfaceSpecular * light.intensities;
    
    // Linear color (color before gamma correction)
    return ambient + attenuation * (diffuse + specular);
}

// Reconstruct position.
vec3 reconstructPos(vec2 texCoord, float depth){
    vec4 sPos = vec4(texCoord * 2.0 - 1.0, depth * 2.0 - 1.0, 1.0);
    sPos = inverseProjectionMatrix * sPos;

    return (sPos.xyz / sPos.w);
}

void main () {
    vec2 texCoords = gl_FragCoord.xy / screenSize;
    
    float depth = texture(tDepth, texCoords).r;
    vec3 position = reconstructPos(texCoords, depth);
    vec3 diffuse = texture(tDiffuse, texCoords).rgb;
    vec3 normal = texture(tNormals, texCoords).xyz;
    vec3 specular = texture(tSpecular, texCoords).xyz;
    
    fragmentColor = vec4(ApplyLight(diffuse, normalize(normal), position, specular), 1.0);
    extraOut = vec4(texture(tGlow, texCoords).rgb, 1.0);
    gl_FragDepth = depth;
}