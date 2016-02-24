#pragma once

class Scene;

namespace System {
    /// %System to update collisions that should reflect
    class ReflectSystem {
    public:
        /// Create reflect system.
        ReflectSystem();
        
        /// Destructor.
        ~ReflectSystem();
        
        /// Update Scene containing entities. Updates life time components.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);
    };
}
