#pragma once

#include <Component/SuperComponent.hpp>

#include <glm/glm.hpp>

namespace Component {
    /// %Component %Explode other entities.
    class Explode : public SuperComponent {
        public:
            /// Create %Explode.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Explode(Entity* entity);
            
            /// Destructor.
            ~Explode();

            /// Time of the explosion (in seconds).
            /**
             * Default: 1.f
             */
            float lifeTime;

            /// Size of the explosion (in seconds).
            /**
             * Default: 1.f
             */
            float size;

            /// Type of particle
            /**
             * Default: FIRE
             */
            int particleTextureIndex;

            /// Offset
            /**
             * Default: 0.f, 0.f, 0.f
             */
            glm::vec3 offset;
            
            /// Whether to play an explosion sound when it explodes.
            bool sound;
    };
}
