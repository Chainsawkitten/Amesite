#include "RelativeTransform.hpp"

#include "../Entity/Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

RelativeTransform::RelativeTransform() : Transform() {
    parentEntity = nullptr;
}

RelativeTransform::~RelativeTransform() {
}

glm::mat4 RelativeTransform::GetModelMatrix() const {
    if (parentEntity != nullptr) {
        return parentEntity->GetComponent<Component::Transform>()->GetModelMatrix() * glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
    }
    return glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
}