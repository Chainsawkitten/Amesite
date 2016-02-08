#pragma once

class Scene;

namespace System {
    /// %System to update damage dealing entities.
    class DamageSystem {
    public:
        /// Create damage system.
        DamageSystem();

        /// Destructor.
        ~DamageSystem();

        /// Update Scene containing entities. Updates hits between damage and health components.
        /**
         * @param scene The scene to update.
         */
        void Update(Scene& scene);

    };

}
