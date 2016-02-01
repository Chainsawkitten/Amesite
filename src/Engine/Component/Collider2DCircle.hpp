#pragma once
#include "Transform.hpp"
#include "../Entity/Entity.hpp"

namespace Component {
    class Collider2DCircle : public SuperComponent {
        public:
            ///Constructor
            Collider2DCircle(Entity* entity);
            ~Collider2DCircle();

            ///Radius of circle
            float radius;
    };
}