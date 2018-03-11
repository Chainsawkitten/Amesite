#include "Sphere.hpp"

namespace Physics {
    Sphere::Sphere(const glm::vec3& position, float radius) {
        this->position = position;
        this->radius = radius;
    }
}
