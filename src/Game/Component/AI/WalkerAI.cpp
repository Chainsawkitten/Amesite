#include "WalkerAI.hpp"

#include <Entity\Entity.hpp>

using namespace Component;

WalkerAI::WalkerAI(Entity* entity) : SuperComponent(entity) {

    target = glm::vec3(10, 0, 0);

}

/// Destructor.
WalkerAI::~WalkerAI() {

}

void WalkerAI::Update(float timeDelta) {

        glm::vec3 dir = normalize(target - entity->GetComponent<Component::Transform>()->position);
        entity->GetComponent<Component::Transform>()->position += dir * timeDelta;

}