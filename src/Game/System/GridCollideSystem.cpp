#include "GridCollideSystem.hpp"

#include "../GameObject/Cave.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <vector>

#include "../Component/GridCollide.hpp"

using namespace System;

GridCollideSystem::GridCollideSystem() {
}

GridCollideSystem::~GridCollideSystem() {
}

void GridCollideSystem::Update(Scene& scene, float deltaTime, GameObject::Cave& cave) {
    std::vector<Component::GridCollide*> gridCollideVector = scene.GetAll<Component::GridCollide>();
    for (auto& gridCollideCompoenent : gridCollideVector)
        if (cave.GridCollide(gridCollideCompoenent->entity, deltaTime)) {
            gridCollideCompoenent->hasCollided = true;
            if (gridCollideCompoenent->removeOnImpact) {
                if (gridCollideCompoenent->entity->gameObject != nullptr)
                    gridCollideCompoenent->entity->gameObject->Kill();
                else
                    gridCollideCompoenent->entity->Kill();
            }
        }
}
