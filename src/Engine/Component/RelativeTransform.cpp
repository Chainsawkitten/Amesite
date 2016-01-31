#include "RelativeTransform.hpp"

#include "../Entity/Entity.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace Component;

RelativeTransform::RelativeTransform(Entity* entity) : Transform(entity) {
    parentEntity = nullptr;
}

RelativeTransform::~RelativeTransform() {
}

void RelativeTransform::UpdateModelMatrix() {
    modelMatrix = CalculateModelMatrix();
}

glm::mat4 RelativeTransform::CalculateModelMatrix() {
    if (parentEntity != nullptr) {
    return parentEntity->GetComponent<Component::Transform>()->modelMatrix * glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
    }
    return glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
}