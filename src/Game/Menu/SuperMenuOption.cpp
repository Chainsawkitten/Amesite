#include "SuperMenuOption.hpp"

#include <glm/gtc/matrix_transform.hpp>

SuperMenuOption::SuperMenuOption(const glm::vec3& position, const glm::vec3& rotation) {
    mPosition = position;
    mRotation = rotation;
}

SuperMenuOption::~SuperMenuOption() {
    
}

glm::mat4 SuperMenuOption::GetModelMatrix() const {
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(mRotation.x), glm::vec3(0.f, 1.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.y), glm::vec3(1.f, 0.f, 0.f));
    orientation = glm::rotate(orientation, glm::radians(mRotation.z), glm::vec3(0.f, 0.f, 1.f));
    
    return glm::translate(glm::mat4(), mPosition) * orientation * glm::scale(glm::mat4(), glm::vec3(GetScale().x, GetScale().y, 1.f));
}
