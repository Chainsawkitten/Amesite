#include "SuperComponent.hpp"

#include "../Entity/Entity.hpp"

using namespace Component;

SuperComponent::SuperComponent(Entity* entity) {
    this->entity = entity;
    mKilled = false;
}

SuperComponent::~SuperComponent() {
    
}

void SuperComponent::Kill() {
    mKilled = true;
}

bool SuperComponent::IsKilled() const {
    return mKilled;
}
