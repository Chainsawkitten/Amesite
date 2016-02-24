#include "LifeTimeSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Explode.hpp"
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
            Component::Explode* explodeComp = lifeTimeComponent->entity->GetComponent<Component::Explode>();
            if (explodeComp != nullptr)
                GameEntityCreator().CreateExplosion(lifeTimeComponent->entity->GetComponent<Component::Transform>()->GetWorldPosition()+explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex); // Create Explosion
                if (lifeTimeComponent->entity->gameObject != nullptr)
                    lifeTimeComponent->entity->gameObject->Clear();
                else
                    lifeTimeComponent->entity->Clear();
        }
    }
}
