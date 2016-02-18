#include "LifeTimeSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Component/LifeTime.hpp"

#include <vector>

using namespace System;

LifeTimeSystem::LifeTimeSystem() {
}

LifeTimeSystem::~LifeTimeSystem() {
}

void LifeTimeSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::LifeTime*> lifeTimeVector = scene.GetAll<Component::LifeTime>();
    for (auto& lifeTimeComponent : lifeTimeVector) {
        lifeTimeComponent->lifeTime -= deltaTime;
        if (lifeTimeComponent->lifeTime < 0.f) {
            //NEVER REMOVE AN ENITY THAT GOT RELATIVE TRANSFROM, WILL LEAD TO EMPTY POINTERS
            if (lifeTimeComponent->entity->gameObject != nullptr)
                lifeTimeComponent->entity->gameObject->Clear();
            else
                lifeTimeComponent->entity->Clear();
        }
    }
}
