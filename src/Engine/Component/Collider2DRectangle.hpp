#pragma once

#include "SuperComponent.hpp"

class Entity;

namespace Component {
    class Collider2DRectangle : public SuperComponent {
        public:
            /// Create new %Collider2DRectangle.
            /**
            * @param entity Pointer to which Entity this %Component corresponds.
            */
            Collider2DRectangle(Entity* entity);

            ///Destructor;
            ~Collider2DRectangle();

            ///width of rectangle
            float width;

            ///height of rectangle
            float height;
    };
}