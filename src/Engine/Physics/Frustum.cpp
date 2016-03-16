#include "Frustum.hpp"

#include "AxisAlignedBoundingBox.hpp"
#include <glm/gtc/matrix_access.hpp>

namespace Physics {
    Frustum::Frustum(const glm::mat4& matrix) {
        // Left clipping plane
        mPlanes[0].x = glm::row(matrix, 3).x + glm::row(matrix, 0).x;
        mPlanes[0].y = glm::row(matrix, 3).y + glm::row(matrix, 0).y;
        mPlanes[0].z = glm::row(matrix, 3).z + glm::row(matrix, 0).z;
        mPlanes[0].w = glm::row(matrix, 3).w + glm::row(matrix, 0).w;
    
        // Right clipping plane
        mPlanes[1].x = glm::row(matrix, 3).x - glm::row(matrix, 0).x;
        mPlanes[1].y = glm::row(matrix, 3).y - glm::row(matrix, 0).y;
        mPlanes[1].z = glm::row(matrix, 3).z - glm::row(matrix, 0).z;
        mPlanes[1].w = glm::row(matrix, 3).w - glm::row(matrix, 0).w;
    
        // Top clipping plane
        mPlanes[2].x = glm::row(matrix, 3).x - glm::row(matrix, 1).x;
        mPlanes[2].y = glm::row(matrix, 3).y - glm::row(matrix, 1).y;
        mPlanes[2].z = glm::row(matrix, 3).z - glm::row(matrix, 1).z;
        mPlanes[2].w = glm::row(matrix, 3).w - glm::row(matrix, 1).w;
    
        // Bottom clipping plane
        mPlanes[3].x = glm::row(matrix, 3).x + glm::row(matrix, 1).x;
        mPlanes[3].y = glm::row(matrix, 3).y + glm::row(matrix, 1).y;
        mPlanes[3].z = glm::row(matrix, 3).z + glm::row(matrix, 1).z;
        mPlanes[3].w = glm::row(matrix, 3).w + glm::row(matrix, 1).w;
    
        // Near clipping plane
        mPlanes[4].x = glm::row(matrix, 3).x + glm::row(matrix, 2).x;
        mPlanes[4].y = glm::row(matrix, 3).y + glm::row(matrix, 2).y;
        mPlanes[4].z = glm::row(matrix, 3).z + glm::row(matrix, 2).z;
        mPlanes[4].w = glm::row(matrix, 3).w + glm::row(matrix, 2).w;
    
        // Far clipping plane
        mPlanes[5].x = glm::row(matrix, 3).x - glm::row(matrix, 2).x;
        mPlanes[5].y = glm::row(matrix, 3).y - glm::row(matrix, 2).y;
        mPlanes[5].z = glm::row(matrix, 3).z - glm::row(matrix, 2).z;
        mPlanes[5].w = glm::row(matrix, 3).w - glm::row(matrix, 2).w;
    }
    
    bool Frustum::Collide(const AxisAlignedBoundingBox& aabb) const {
        // Define the AABB's vertices.
        glm::vec3 vertices[8];
        vertices[0] = aabb.minVertex;
        vertices[1] = aabb.minVertex + glm::vec3(aabb.dimensions.x, 0.f, 0.f);
        vertices[2] = aabb.minVertex + glm::vec3(0.f, aabb.dimensions.y, 0.f);
        vertices[3] = aabb.minVertex + glm::vec3(0.f, 0.f, aabb.dimensions.z);
        vertices[4] = aabb.maxVertex;
        vertices[5] = aabb.maxVertex - glm::vec3(aabb.dimensions.x, 0.f, 0.f);
        vertices[6] = aabb.maxVertex - glm::vec3(0.f, aabb.dimensions.y, 0.f);
        vertices[7] = aabb.maxVertex - glm::vec3(0.f, 0.f, aabb.dimensions.z);
    
        // Check if the AABB's vertices lie in the planes or in their positive halfspaces.
        // Only one vertex has to lie inside the halfspace for each plane.
        for (int plane = 0; plane < 6; plane++) {
            bool inside = false;
            for (int vertex = 0; vertex < 8; vertex++) {
                if (DistanceToPoint(mPlanes[plane], vertices[vertex]) >= 0.f) {
                    inside = true;
                    break;
                }
            }
    
            if (!inside)
                return false;
        }
        return true;
    }
    
    float Frustum::DistanceToPoint(const glm::vec4& plane, const glm::vec3& point) {
        return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
    }
}
