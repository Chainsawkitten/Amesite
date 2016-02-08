#pragma once

class Scene;

namespace System {
    /// %System to update the life time of entities and remove them is there life time is out.
    class LifeTimeSystem {
    public:
        /// Create life time system.
        LifeTimeSystem();
        
        /// Destructor.
        ~LifeTimeSystem();
        
        /// Update Scene containing entities. Updates life time components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);
    };
}
