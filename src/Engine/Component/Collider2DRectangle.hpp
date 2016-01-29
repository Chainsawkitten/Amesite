#pragma once
#include "Transform.hpp"
#include "../Entity/Entity.hpp"

namespace Component {
    class Collider2DRectangle : public Component {
        public:
            Collider2DRectangle(float width, float height, Transform* mTransform);
            ~Collider2DRectangle();
            bool CollideRectangle(Entity* other);
            bool CollideCircle(Entity* other);
            float width, height;
            Entity *parent;
    };
}