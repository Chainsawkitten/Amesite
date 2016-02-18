#include "SuperGameObject.hpp"

#include <Engine/Entity/Entity.hpp>

using namespace GameObject;

SuperGameObject::SuperGameObject(Scene* scene) {
    mScene = scene;
    mScene->GetVector<SuperGameObject>()->push_back(this);
}

SuperGameObject::~SuperGameObject() {
}

void SuperGameObject::Clear() {
    for (auto& entity : mEntityVector)
        entity->Clear();
    mEntityVector.clear();
}

Entity* SuperGameObject::CreateEntity(Scene* scene) {
    Entity* entity = scene->CreateEntity();
    entity->gameObject = this;
    mEntityVector.push_back(entity);
    return entity;
}