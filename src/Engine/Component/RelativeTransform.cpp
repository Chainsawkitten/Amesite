#include "RelativeTransform.hpp"

#include "../Entity/Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

RelativeTransform::RelativeTransform() : Transform() {
    parentEntity = nullptr;
}

RelativeTransform::~RelativeTransform() {
}

glm::mat4 RelativeTransform::GetOrientation() const
{
    glm::mat4 orientation;
    orientation = glm::rotate(orientation, glm::radians(yaw), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(pitch), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(roll), glm::vec3(0, 0, 1));

    if (parentEntity != nullptr) {
        parentEntity->GetComponent<Component::Transform>()->GetOrientation() * ;
    }
    return orientation;
}