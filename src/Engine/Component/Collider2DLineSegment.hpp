#pragma once
#include <glm/glm.hpp>
#include "Transform.hpp"

namespace Component {
    class Collider2DLineSegment {
        public:
            Collider2DLineSegment(glm::vec2 pointA, glm::vec2, Transform* mTransform);
            ~Collider2DLineSegment();
            glm::vec2 pointA, pointB;
        private:
            Transform* mTransform;
    };
}