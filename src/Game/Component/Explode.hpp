#pragma once

#include <Component/SuperComponent.hpp>

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
    };
}
