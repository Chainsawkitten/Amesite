#include "WalkerAI.hpp"

#include <Entity/Entity.hpp>

using namespace Component;

WalkerAI::WalkerAI(Entity* entity) : SuperComponent(entity) {

    target = glm::vec3(0, 0, 0);

}

/// Destructor.
WalkerAI::~WalkerAI() {

}

void WalkerAI::Update(float deltaTime) {
        glm::vec3 dir = glm::normalize(target - entity->GetComponent<Component::Transform>()->position);
        entity->GetComponent<Component::Transform>()->position += dir * deltaTime;
}
