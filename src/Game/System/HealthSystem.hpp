#pragma once

class Scene;

namespace System {
    /// System to update the physics of a scene.
    class HealthSystem {
    public:
        /// Create physics system.
        HealthSystem();

        /// Destructor.
        ~HealthSystem();

        /// Update Scene containing entities. Updates Health.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);

    };

}
