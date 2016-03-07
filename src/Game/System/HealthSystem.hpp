#pragma once

#include <vector>

class Scene;
namespace Component {
    class Health;
}

namespace System {
    /// %System to update the health of entities and set killed entities on cooldown.
    class HealthSystem {
        public:
            /// Create health system.
            HealthSystem();
            
            /// Destructor.
            ~HealthSystem();
            
            /// Update Scene containing entities. Updates health.
            /**
             * @param scene The scene to update.
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(Scene& scene, float deltaTime);
            
        private:
            float mDeltaTime;
            
            // Update part of the component vector.
            void UpdatePart(std::vector<Component::Health*>& healths, std::size_t begin, std::size_t length);
    };
}
