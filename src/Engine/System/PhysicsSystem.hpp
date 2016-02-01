#pragma once

class Scene;

#include <glm/glm.hpp>

namespace System {
    /// System to update the physics of a scene.
    class PhysicsSystem {
        public:
            /// Create new physics system.
            PhysicsSystem(glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f));

            /// Destructor.
            ~PhysicsSystem();

            /// Update Scene containing entities. Moves entities and updates the physics component.
            /**
             * @param scene Contains a bunch of entities.
             * @param timeDelta Time (in secounds).
             */
            void Update(const Scene& scene, float timeDelta);
        private:
            /// Gravity (in meter/secound)
            /**
             * Default: 0.f, -9.82.f, 0.f
             */
            glm::vec3 mGravity;
        };
}
