/*
Calculate which lights contribute to this tile.
*/
#version 400

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

const uint tileSize = 8;
const uint maxTileLights = 16;

out uint lightIndex;

bool lightVisible(uint light) {
    // TODO: Frustum check.
    return true;
}

void main () {
    lightIndex = uint(lights[0].padding1);
    
    // TODO: Figure out which tile we are.
    
    // Which light number to fetch.
    uint maxLightNumber = uint(gl_FragCoord.y) % maxTileLights;
    
    // Check which lights contribute to this tile.
    uint lightNumber = 0;
    for (uint i=0; i < lightCount; ++i) {
        if (lightVisible(i)) {
            if (lightNumber >= maxLightNumber) {
                lightIndex = i + 1;
                break;
            }
            
            ++lightNumber;
        }
    }
}
