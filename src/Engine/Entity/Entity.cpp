#include "Entity.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;
}

Entity::~Entity() {
    //Clear();
}

void Entity::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    this->components[componentType] = component;
}

void Entity::Clear() {
    for (auto& it : components) {
        delete it.second;
        mScene->RemoveComponentFromList(it.second, it.first);
    }

    components.clear();
}