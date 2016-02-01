#pragma once
#include "Transform.hpp"
#include "../Entity/Entity.hpp"

namespace Component {
    class Collider2DRectangle : public SuperComponent {
        public:
            ///Constructor
            Collider2DRectangle(Entity* entity);
            ~Collider2DRectangle();

            ///width of rectangle
            float width;

            ///height of rectangle
            float height;
    };
}