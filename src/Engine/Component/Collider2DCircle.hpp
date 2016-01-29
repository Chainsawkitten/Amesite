#pragma once
#include "Transform.hpp"

namespace Component {
    class Collider2DCircle {
        public:
            Collider2DCircle(float radius, Transform* mTransform);
            ~Collider2DCircle();
            float radius;
        private:
            Transform* transform;
    };
}