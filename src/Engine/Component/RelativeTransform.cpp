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
    if (parentEntity != nullptr) {
        orientationMatrix = parentEntity->GetComponent<Component::Transform>()->orientationMatrix * GetOrientation();
        modelMatrix = parentEntity->GetComponent<Component::Transform>()->modelMatrix * glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
    } else {
        orientationMatrix = GetOrientation();
        modelMatrix = glm::translate(glm::mat4(), position) * orientationMatrix * glm::scale(glm::mat4(), scale);
    }
}

glm::vec3 RelativeTransform::GetWorldScale() const {
    if (parentEntity != nullptr)
        return parentEntity->GetComponent<Component::Transform>()->GetWorldScale() * scale;
    else
        return scale;
}

glm::vec3 RelativeTransform::GetWorldRotation() const {
    if (parentEntity != nullptr)
        return parentEntity->GetComponent<Component::Transform>()->GetWorldRotation() + glm::vec3(yaw, pitch, roll);
    else
        return glm::vec3(yaw, pitch, roll);
}