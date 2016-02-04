#include "Entity.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;
}

Entity::~Entity() {
}

void Entity::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    this->components[componentType] = component;
}
