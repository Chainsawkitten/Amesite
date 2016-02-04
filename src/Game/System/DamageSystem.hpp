#pragma once

class Scene;

namespace System {
    /// System to update the physics of a scene.
    class DamageSystem {
    public:
        /// Create physics system.
        DamageSystem();

        /// Destructor.
        ~DamageSystem();

        /// Update Scene containing entities. Updates hits between damage and health componenets.
        /**
         * @param scene The scene to update.
         */
        void Update(Scene& scene);

    };

}
