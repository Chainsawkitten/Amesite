#pragma once

class Scene;

namespace System {
    /// %System to explode killed entites.
    class ExplodeSystem {
    public:
        /// Create explode system.
        ExplodeSystem();
        
        /// Destructor.
        ~ExplodeSystem();
        
        /// Update Scene containing entities. Explodes killed entites.
        /**
         * @param scene The scene to update.
         */
        void Update(Scene& scene);
    };
}
