#pragma once

#include <vector>

class Scene;
namespace Component {
    class Spawner;
}

namespace System {
    /// %System to update the timer of spawner
    class SpawnerSystem {
        public:
            /// Create spawner system.
            SpawnerSystem();
            
            /// Destructor.
            ~SpawnerSystem();
            
            /// Update scene containing entities. Updates life time components.
            /**
             * @param scene The scene to update.
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(Scene& scene, float deltaTime);
            
        private:
            // Update part of the component vector.
            void UpdatePart(std::vector<Component::Spawner*>& spawners, float deltaTime, std::size_t begin, std::size_t length);
    };
}
