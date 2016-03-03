#pragma once

class Scene;

namespace System {
    /// %System to update the timer of spawner
    class SpawnerSystem {
    public:
        /// Create spawner system.
        SpawnerSystem();
        
        /// Destructor.
        ~SpawnerSystem();
        
        /// Update Scene containing entities. Updates life time components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);
    };
}
