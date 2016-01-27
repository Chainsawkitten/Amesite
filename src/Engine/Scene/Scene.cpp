#pragma once

#include "Scene.hpp"

#include <Engine/Entity/Entity.hpp>

Scene::Scene() {

}

Scene::~Scene() {
    for (unsigned int i = 0; i < mEntityVec.size(); i++) {
        delete mEntityVec.at(i);
    }
    mEntityVec.clear();
    mEntityVec.shrink_to_fit();
}

Entity* Scene::CreateEntity() {
    Entity* entity = new Entity();
    mEntityVec.push_back(entity);
    return entity;
}

unsigned int Scene::Size() {
    return mEntityVec.size();
}

void Scene::Clear() {
    for (unsigned int i = 0; i < mEntityVec.size(); i++) {
        delete mEntityVec.at(i);
    }
    mEntityVec.clear();
    mEntityVec.shrink_to_fit();
}

Entity* Scene::operator[](unsigned int index) {
    if (index < mEntityVec.size()) {
        return mEntityVec.at(index);
    }
    return nullptr;
}
