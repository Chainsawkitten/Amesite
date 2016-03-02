#pragma once

class Scene;

namespace GameObject {
    class Cave;
}

namespace System {
    /// %System to check if entity collide with map
    class GridCollideSystem {
    public:
        /// Create grid collide system.
        GridCollideSystem();
        
        /// Destructor.
        ~GridCollideSystem();
        
        /// Update Scene containing entities. Updates life time components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         * @param cave Cave to check collision.
         */
        void Update(Scene& scene, float deltaTime, GameObject::Cave& cave);
    };
}
