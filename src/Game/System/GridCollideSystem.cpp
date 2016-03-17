#include "GridCollideSystem.hpp"

#include "../GameObject/Cave.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include <vector>

#include "Component/Physics.hpp"

#include "../Component/GridCollide.hpp"
#include "../Component/Explode.hpp"

using namespace System;

GridCollideSystem::GridCollideSystem() {
}

GridCollideSystem::~GridCollideSystem() {
}

void GridCollideSystem::Update(Scene& scene, float deltaTime, GameObject::Cave& cave) {
    std::vector<Component::GridCollide*> gridCollideVector = scene.GetAll<Component::GridCollide>();
    for (auto& gridCollideComponent : gridCollideVector) {

        if (glm::length(gridCollideComponent->entity->GetComponent<Component::Physics>()->velocity) < 0.001f)
            continue;

        bool collide = false;
        if (gridCollideComponent->removeOnImpact)
            collide = cave.WallIntersect(gridCollideComponent->entity, deltaTime);
        else collide = cave.GridCollide(gridCollideComponent->entity, deltaTime);

        if (collide) {
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
}
