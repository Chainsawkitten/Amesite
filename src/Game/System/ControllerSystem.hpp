#pragma once

class Scene;

namespace System {
    /// System to update the physics of a scene.
    class ControllerSystem {
    public:
        /// Create physics system.
        ControllerSystem();

        /// Destructor.
        ~ControllerSystem();

        /// Update Scene containing entities. Moves entities and updates the physics component.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);

    };

}
