#pragma once

class Scene;

namespace GameObject {
    class Cave;
}

namespace System {
    /// %System to check if entity collide with terrain
    class GroundCollideSystem {
    public:
        /// Create ground collide system.
        GroundCollideSystem();
        
        /// Destructor.
        ~GroundCollideSystem();
        
        /// Update Scene containing entities.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         * @param cave Cave to check collision.
         */
        void Update(Scene& scene, float deltaTime, GameObject::Cave& cave);
    };
}
