#include "Entity.hpp"

#include <algorithm>

#include "../GameObject/SuperGameObject.hpp"

Entity::Entity(Scene* scene) {
    mScene = scene;
    gameObject = nullptr;
}

Entity::~Entity() {
}

void Entity::AddComponent(Component::SuperComponent* component, const std::type_info* componentType) {
    this->components[componentType] = component;
}

void Entity::Clear() {
    for (auto& it : components) {
        mScene->RemoveComponentFromList(it.second, it.first);
        if (it.first != &typeid(Component::RelativeTransform*))
            delete it.second;
    }
    components.clear();
    std::vector<Entity*>* entityVector = mScene->GetVector<Entity>();
    entityVector->erase(std::remove(entityVector->begin(), entityVector->end(), this), entityVector->end());
    delete this;
}

void Entity::Kill() {
    if (std::find(mScene->mKilledEntites.begin(), mScene->mKilledEntites.end(), this) == mScene->mKilledEntites.end())
        mScene->mKilledEntites.push_back(this);
}