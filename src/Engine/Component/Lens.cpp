#include "Lens.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

using namespace Component;

Lens::Lens() {
    mFieldOfView = 45.f;
    mZFar = 100.f;
    mZNear = 0.05f;
}

Lens::~Lens() {

}

glm::mat4 Lens::Projection(const glm::vec2& screenSize) const {
    return glm::perspective(glm::radians(mFieldOfView), screenSize.x/screenSize.y, mZNear, mZFar);
}
