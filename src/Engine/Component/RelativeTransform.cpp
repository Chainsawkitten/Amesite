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
        worldOrientationMatrix = worldOrientationMatrix * GetLocalOrientation();
        modelMatrix = parentEntity->GetComponent<Component::Transform>()->modelMatrix * glm::translate(glm::mat4(), position) * GetLocalOrientation() * glm::scale(glm::mat4(), scale);
    }
    else {
        worldOrientationMatrix = GetLocalOrientation();
        modelMatrix = glm::translate(glm::mat4(), position) * worldOrientationMatrix * glm::scale(glm::mat4(), scale);
    }
}

glm::vec3 RelativeTransform::CalculateWorldPosition() const {
    if (parentEntity != nullptr)
        glm::vec3 relativePosition = (position + parentEntity->GetComponent<Component::Transform>()->CalculateWorldPosition()) * scale;
    else
        return position;
}

glm::vec3 RelativeTransform::GetWorldScale() const {
    if (parentEntity != nullptr)
        return parentEntity->GetComponent<Component::Transform>()->GetWorldScale() * scale;
    else
        return scale;
}

glm::vec3 RelativeTransform::GetWorldYawPitchRoll() const {
    if (parentEntity != nullptr)
        return parentEntity->GetComponent<Component::Transform>()->GetWorldYawPitchRoll() + glm::vec3(yaw, pitch, roll);
    else
        return glm::vec3(yaw, pitch, roll);
}

glm::vec3 RelativeTransform::GetWorldDirection() const {
    return glm::vec3(glm::normalize(GetWorldOrientation() * glm::vec4(0, 0, 1, 0)));
}

glm::mat4 RelativeTransform::GetWorldOrientation() const {
    glm::mat4 orientation;
    glm::vec3 yawPitchRoll = GetWorldYawPitchRoll();
    orientation = glm::rotate(orientation, glm::radians(yawPitchRoll.x), glm::vec3(0, 1, 0));
    orientation = glm::rotate(orientation, glm::radians(yawPitchRoll.y), glm::vec3(1, 0, 0));
    orientation = glm::rotate(orientation, glm::radians(yawPitchRoll.z), glm::vec3(0, 0, 1));
    return orientation;
}