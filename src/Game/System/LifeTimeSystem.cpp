#include "LifeTimeSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Explode.hpp"
#include "../Component/LifeTime.hpp"

using namespace System;

LifeTimeSystem::LifeTimeSystem() {
}

LifeTimeSystem::~LifeTimeSystem() {
}

void LifeTimeSystem::Update(Scene& scene, float deltaTime) {
    std::list<Component::LifeTime*> lifeTimes = scene.GetAll<Component::LifeTime>();
    for (auto& lifeTimeComponent : lifeTimes) {
        lifeTimeComponent->lifeTime -= deltaTime;
        if (lifeTimeComponent->lifeTime < 0.f)
            if (lifeTimeComponent->entity->gameObject != nullptr)
                lifeTimeComponent->entity->gameObject->Kill();
            else
                lifeTimeComponent->entity->Kill();
    }
}
