#pragma once

class Scene;

namespace System {
    /// %System to update the life time of entities and remove them is there life time is out.
    class AISystem {
    public:
        /// Create the AI system.
        AISystem();
        
        /// Destructor.
        ~AISystem();
        
        /// Update Scene containing entities. Updates AI components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);
    };
}
