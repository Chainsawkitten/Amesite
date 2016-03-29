#include "Entity.hpp"

#include <algorithm>

#include "../GameObject/SuperGameObject.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;
    gameObject = nullptr;
    mKilled = false;
}

Entity::~Entity() {
}

void Entity::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    components[componentType] = component;
}

void Entity::Kill() {
    mKilled = true;
    
    for (auto& it : components)
        it.second->Kill();
}

bool Entity::IsKilled() const {
    return mKilled;
}
