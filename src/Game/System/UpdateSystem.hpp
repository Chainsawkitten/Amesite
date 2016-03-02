#pragma once

class Scene;

namespace System {
    /// %System to run update function
    class UpdateSystem {
    public:
        /// Create update system.
        UpdateSystem();
        
        /// Destructor.
        ~UpdateSystem();
        
        /// Update Scene containing entities. Updates life time components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);
    };
}
