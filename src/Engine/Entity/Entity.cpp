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

void Entity::Clear() {
    /*for (auto& it : components) {
        mScene->RemoveComponentFromList(it.second, it.first);
        if (it.first != &typeid(Component::RelativeTransform*))
            delete it.second;
    }
    components.clear();*/
    std::list<Entity*>* entities = mScene->GetList<Entity>();
    entities->erase(std::remove(entities->begin(), entities->end(), this), entities->end());
    delete this;
}

void Entity::Kill() {
    mKilled = true;
    
    for (auto& it : components)
        it.second->Kill();
    components.clear();
    
    if (std::find(mScene->mKilledEntites.begin(), mScene->mKilledEntites.end(), this) == mScene->mKilledEntites.end())
        mScene->mKilledEntites.push_back(this);
}

bool Entity::IsKilled() const {
    return mKilled;
}
