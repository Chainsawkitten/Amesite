#include "SuperGameObject.hpp"

#include <Engine/Entity/Entity.hpp>

#include <algorithm>

using namespace GameObject;

SuperGameObject::SuperGameObject(Scene* scene) {
    mScene = scene;
    mScene->GetList<SuperGameObject>()->push_back(this);
    mKilled = false;
}

SuperGameObject::~SuperGameObject() {
}

void SuperGameObject::Kill() {
    mKilled = true;
    
    for (auto entity : mEntityVector)
        entity->Kill();
}

bool SuperGameObject::IsKilled() const {
    return mKilled;
}

Entity* SuperGameObject::CreateEntity() {
    Entity* entity = mScene->CreateEntity();
    entity->gameObject = this;
    mEntityVector.push_back(entity);
    return entity;
}
