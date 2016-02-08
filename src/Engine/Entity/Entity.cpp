#include "Entity.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;
}

Entity::~Entity() {
}

void Entity::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    this->components[componentType] = component;
}

void Entity::Clear() {
    for (auto& it : components) {
        mScene->RemoveComponentFromList(it.second, it.first);
        delete it.second;
    }
    components.clear();
}