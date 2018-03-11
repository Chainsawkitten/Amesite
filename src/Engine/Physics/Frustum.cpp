#include "Frustum.hpp"

#include "AxisAlignedBoundingBox.hpp"
#include <glm/gtc/matrix_access.hpp>

namespace Physics {
    Frustum::Frustum(const glm::mat4& matrix) {
        // Left clipping plane
        mPlanes[0] = glm::row(matrix, 3) + glm::row(matrix, 0);
        mPlanes[0] /= glm::length(glm::vec3(mPlanes[0]));
    
        // Right clipping plane
        mPlanes[1] = glm::row(matrix, 3) - glm::row(matrix, 0);
        mPlanes[1] /= glm::length(glm::vec3(mPlanes[1]));
    
        // Top clipping plane
        mPlanes[2] = glm::row(matrix, 3) - glm::row(matrix, 1);
        mPlanes[2] /= glm::length(glm::vec3(mPlanes[2]));
    
        // Bottom clipping plane
        mPlanes[3] = glm::row(matrix, 3) + glm::row(matrix, 1);
        mPlanes[3] /= glm::length(glm::vec3(mPlanes[3]));
    
        // Near clipping plane
        mPlanes[4] = glm::row(matrix, 3) + glm::row(matrix, 2);
        mPlanes[4] /= glm::length(glm::vec3(mPlanes[4]));
    
        // Far clipping plane
        mPlanes[5] = glm::row(matrix, 3) - glm::row(matrix, 2);
        mPlanes[5] /= glm::length(glm::vec3(mPlanes[5]));
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
