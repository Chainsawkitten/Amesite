#include "Frustum.hpp"

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
    
    float Frustum::DistanceToPoint(const glm::vec4& plane, const glm::vec3& point) {
        return plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
    }
}
