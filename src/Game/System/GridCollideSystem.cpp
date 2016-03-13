#include "GridCollideSystem.hpp"

#include "../GameObject/Cave.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <vector>

#include "../Component/GridCollide.hpp"
#include "../Component/Explode.hpp"

using namespace System;

GridCollideSystem::GridCollideSystem() {
}

GridCollideSystem::~GridCollideSystem() {
}

void GridCollideSystem::Update(Scene& scene, float deltaTime, GameObject::Cave& cave) {
    std::vector<Component::GridCollide*> gridCollideVector = scene.GetAll<Component::GridCollide>();
    for (auto& gridCollideComponent : gridCollideVector)
        if (cave.GridCollide(gridCollideComponent->entity, deltaTime)) {
            gridCollideComponent->hasCollided = true;
            Component::Explode* explode = gridCollideComponent->entity->GetComponent<Component::Explode>();
            if (explode != nullptr)
                explode->type = Component::Explode::TYPE::CAVE;
            if (gridCollideComponent->removeOnImpact) {
                if (gridCollideComponent->entity->gameObject != nullptr)
                    gridCollideComponent->entity->gameObject->Kill();
                else
                    gridCollideComponent->entity->Kill();
            }
        }
}
