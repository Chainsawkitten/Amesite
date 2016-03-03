#include "ExplodeSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Explode.hpp"
#include <Engine/Component/Transform.hpp>

#include "../Util/GameEntityFactory.hpp"

#include <vector>

using namespace System;

ExplodeSystem::ExplodeSystem() {
}

ExplodeSystem::~ExplodeSystem() {
}

void ExplodeSystem::Update(Scene& scene) {
    std::vector<Entity*> killedEntitiesVector = scene.GetKilledEntitesVector();
    for (auto entity : killedEntitiesVector) {
        Component::Explode* explodeComp = entity->GetComponent<Component::Explode>();
        if (explodeComp != nullptr)
            GameEntityCreator().CreateExplosion(entity->GetComponent<Component::Transform>()->GetWorldPosition() + explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex);
    }
}
