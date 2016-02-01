#pragma once

#include "SuperComponent.hpp"

class Entity;

namespace Component {
    class Collider2DCircle : public SuperComponent {
        public:
            /// Create new %Collider2DCircle.
            /**
            * @param entity Pointer to which Entity this %Component corresponds.
            */
            Collider2DCircle(Entity* entity);

            ///Destructor
            ~Collider2DCircle();

            ///Radius of circle
            float radius;
    };
}