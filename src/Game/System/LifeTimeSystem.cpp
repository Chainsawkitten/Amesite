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
            // Create Explosion
            if (explodeComp != nullptr)
                GameEntityCreator().CreateExplosion(lifeTimeComponent->entity->GetComponent<Component::Transform>()->GetWorldPosition()+explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex);
            //Kill game object
            lifeTimeComponent->entity->gameObject->Kill();
        }
    }
}
