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
}
