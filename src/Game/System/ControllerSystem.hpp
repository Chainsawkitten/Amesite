#pragma once

class Scene;

namespace System {
    /// %System to let the user control entities in a scene.
    class ControllerSystem {
    public:
        /// Update Scene containing entities. Moves entities according to player input.
        /**
         * @param scene The scene to update.
         * @param deltaTime Time since last frame (in seconds).
         */
        void Update(Scene& scene, float deltaTime);

    };

}
