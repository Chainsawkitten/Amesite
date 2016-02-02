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
    if (parentEntity != nullptr)
        modelMatrix = parentEntity->GetComponent<Component::Transform>()->modelMatrix * glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
    else
        modelMatrix = glm::translate(glm::mat4(), position) * GetOrientation() * glm::scale(glm::mat4(), scale);
}
