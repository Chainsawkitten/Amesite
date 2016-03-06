#include "GridCollideSystem.hpp"

#include "../GameObject/Cave.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include "../Component/GridCollide.hpp"

#include <Threading/Threading.hpp>

using namespace System;

GridCollideSystem::GridCollideSystem() {
}

GridCollideSystem::~GridCollideSystem() {
}

void GridCollideSystem::Update(Scene& scene, float deltaTime, GameObject::Cave& cave) {
    std::vector<Component::GridCollide*>& gridCollideVector = scene.GetAll<Component::GridCollide>();
    
    mDeltaTime = deltaTime;
    mCave = &cave;
    Threading::ParallelFor(&GridCollideSystem::UpdatePart, this, gridCollideVector);
    Threading::FrontEndJobs().Wait();
}

void GridCollideSystem::UpdatePart(std::vector<Component::GridCollide*>& gridCollides, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i) {
        if (mCave->GridCollide(gridCollides[i]->entity, mDeltaTime))
            if (gridCollides[i]->entity->gameObject != nullptr)
                gridCollides[i]->entity->gameObject->Kill();
            else
                gridCollides[i]->entity->Kill();
    }
}
