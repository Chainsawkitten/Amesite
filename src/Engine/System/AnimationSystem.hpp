#pragma once

class Entity;
class Scene;

namespace System {
    /// %System to animate.
    class AnimationSystem {
        public:
            /// Create animation system.
            AnimationSystem();

            /// Destructor.
            ~AnimationSystem();

            /// Update Scene containing entities.
            /**
             * @param scene Contains a bunch of entities.
             */
            void Update(Scene& scene, float deltaTime);
    };
}
