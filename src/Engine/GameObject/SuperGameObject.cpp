#include "SuperGameObject.hpp"

#include <Engine/Entity/Entity.hpp>

using namespace GameObject;

SuperGameObject::SuperGameObject(Scene* scene) {
    mScene = scene;
}

SuperGameObject::~SuperGameObject() {
    Clear();
}

void SuperGameObject::Clear() {
    for (auto& entity : mEntityVector)
        entity->Clear();
    mEntityVector.clear();

    mEntityMap.clear();
}

Entity* SuperGameObject::GetEntity(const std::string key) {
    std::map<std::string, Entity*>::iterator it = mEntityMap.find(key);
    if (it != mEntityMap.end())
        return it->second;
    return nullptr;
}
