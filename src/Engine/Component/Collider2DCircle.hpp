#pragma once

#include "SuperComponent.hpp"

class Entity;


namespace Component {
    ///Component describing a circle.
    class Collider2DCircle : public SuperComponent {
        public:
            /// Create new %Collider2DCircle.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Collider2DCircle(Entity* entity);

            /// Destructor.
            ~Collider2DCircle();

            /// Radius of circle.
            /**
             * Default: 1.f
             */
            float radius;
    };
}
