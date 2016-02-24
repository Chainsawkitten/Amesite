#include "SuperGameObject.hpp"

#include <Engine/Entity/Entity.hpp>

#include <algorithm>

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
    std::vector<GameObject::SuperGameObject*>* gameObjectVector = mScene->GetVector<SuperGameObject>();
    gameObjectVector->erase(std::remove(gameObjectVector->begin(), gameObjectVector->end(), this), gameObjectVector->end());
    delete this;
}

void SuperGameObject::Kill() {
    mScene->AddKilledGameObject(this);
}

Entity* SuperGameObject::CreateEntity(Scene* scene) {
    Entity* entity = scene->CreateEntity();
    entity->gameObject = this;
    mEntityVector.push_back(entity);
    return entity;
}