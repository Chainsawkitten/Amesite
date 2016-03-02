#include "SuperGameObject.hpp"

#include <Engine/Entity/Entity.hpp>

#include <algorithm>

using namespace GameObject;

SuperGameObject::SuperGameObject(Scene* scene) {
    mScene = scene;
    mScene->GetList<SuperGameObject>()->push_back(this);
}

SuperGameObject::~SuperGameObject() {
}

void SuperGameObject::Clear() {
    for (auto& entity : mEntityVector)
        entity->Clear();
    mEntityVector.clear();
    std::list<SuperGameObject*>* gameObjects = mScene->GetList<SuperGameObject>();
    gameObjects->erase(std::remove(gameObjects->begin(), gameObjects->end(), this), gameObjects->end());
    delete this;
}

void SuperGameObject::Kill() {
    for (auto entity : mEntityVector) {
        entity->Kill();
        entity->gameObject = nullptr;
    }
    std::list<SuperGameObject*>* gameObjects = mScene->GetList<SuperGameObject>();
    gameObjects->erase(std::remove(gameObjects->begin(), gameObjects->end(), this), gameObjects->end());
    delete this;
}

Entity* SuperGameObject::CreateEntity() {
    Entity* entity = mScene->CreateEntity();
    entity->gameObject = this;
    mEntityVector.push_back(entity);
    return entity;
}
