#pragma once

#include <glm/glm.hpp>
#include <vector>

class Scene;
namespace Component {
    class Physics;
}

namespace System {
    /// %System to update the physics of a scene.
    class PhysicsSystem {
        public:
            /// Create physics system.
            PhysicsSystem(glm::vec3 gravity = glm::vec3(0.f, -9.82f, 0.f));

            /// Destructor.
            ~PhysicsSystem();

            /// Update Scene containing entities. Moves entities and updates the physics component.
            /**
             * @param scene The scene to update.
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(Scene& scene, float deltaTime);
            
        private:
            // Gravity (in meter/second)
            /*
             * Default: 0.f, -9.82.f, 0.f
             */
            glm::vec3 mGravity;
            
            float mDeltaTime;
            
            // Update part of the component vector.
            void UpdatePart(std::vector<Component::Physics*>& physics, std::size_t begin, std::size_t length);
        };
}
