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

uniform mat4 view;
uniform mat4 inverseView;
uniform mat4 projection;

const uint tileSize = 8;
const uint maxTileLights = 16;

out uint lightIndex;

// Sphere using during frustum-sphere intersection test.
struct Sphere {
    vec3 position;
    float radius;
};

// Frustum using during frustum-sphere intersection test.
struct Frustum {
    vec4 planes[6];
};

// Get a row from a matrix.
vec4 row(in mat4 matrix, in int number) {
    return vec4(matrix[0][number], matrix[1][number], matrix[2][number], matrix[3][number]);
}

// Extract the frustum planes from a view projection matrix.
void ExtractFrustum(in mat4 matrix, out Frustum frustum) {
    // Left clipping plane
    frustum.planes[0] = row(matrix, 3) + row(matrix, 0);
    frustum.planes[0] /= length(vec3(frustum.planes[0]));
    
    // Right clipping plane
    frustum.planes[1] = row(matrix, 3) - row(matrix, 0);
    frustum.planes[1] /= length(vec3(frustum.planes[1]));
    
    // Top clipping plane
    frustum.planes[2] = row(matrix, 3) - row(matrix, 1);
    frustum.planes[2] /= length(vec3(frustum.planes[2]));
    
    // Bottom clipping plane
    frustum.planes[3] = row(matrix, 3) + row(matrix, 1);
    frustum.planes[3] /= length(vec3(frustum.planes[3]));
    
    // Near clipping plane
    frustum.planes[4] = row(matrix, 3) + row(matrix, 2);
    frustum.planes[4] /= length(vec3(frustum.planes[4]));
    
    // Far clipping plane
    frustum.planes[5] = row(matrix, 3) - row(matrix, 2);
    frustum.planes[5] /= length(vec3(frustum.planes[5]));
}

// Frustum-sphere intersection test.
bool FrustumVsSphere(in Frustum frustum, in Sphere sphere) {
    // Check the signed distance between the sphere and the frustum planes.
    for (int planeNumber = 0; planeNumber < 6; ++planeNumber) {
        vec4 plane = frustum.planes[planeNumber];
        vec3 point = sphere.position;
        float signedDistance = plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
        
        if (signedDistance < -sphere.radius)
            return false;
    }
    
    return true;
}

// Determine whether a light is visible inside a given tile using frustum culling.
bool LightVisible(in Frustum frustum, in uint light) {
    // Don't frustum cull directional and spot lights.
    const float epsilon = 0.01;
    if (lights[light].position.w == 0.0)// || lights[light].coneAngle < 180.0 + epsilon)
        return true;
    
    // Get light info.
    Sphere lightSphere;
    lightSphere.position = vec3(inverseView * lights[light].position);
    lightSphere.radius = lights[light].distance;
    
    // Frustum test.
    return FrustumVsSphere(frustum, lightSphere);
}

void main () {
    lightIndex = 0;
    
    // TODO: Figure out which tile we are.
    
    // Calculate view projection matrix.
    // TODO: Adjust depending on which tile we're in.
    mat4 viewProjection = projection * view;
    
    // Extract frustum from view projection matrix.
    Frustum frustum;
    ExtractFrustum(viewProjection, frustum);
    
    // Which light number to fetch.
    uint maxLightNumber = uint(gl_FragCoord.y) % maxTileLights;
    
    // Check which lights contribute to this tile.
    uint lightNumber = 0;
    for (uint i=0; i < lightCount; ++i) {
        if (LightVisible(frustum, i)) {
            if (lightNumber >= maxLightNumber) {
                lightIndex = i + 1;
                break;
            }
            
            ++lightNumber;
        }
    }
}
