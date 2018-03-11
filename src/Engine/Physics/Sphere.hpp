#pragma once

#include <glm/glm.hpp>

namespace Physics {
    /// A sphere.
    /**
     * Used for intersection testing.
     */
    class Sphere {
        public:
            /// Origin.
            glm::vec3 position;
            
            /// Radius.
            float radius;
            
            /// Create new sphere.
            /**
             * @param position The origin of the sphere.
             * @param radius The radius of the sphere.
             */
            Sphere(const glm::vec3& position, float radius);
    };
}
