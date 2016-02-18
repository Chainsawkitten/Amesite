#include "AISystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/AI/WalkerAI.hpp"

#include <vector>

using namespace System;

AISystem::AISystem() {
}

AISystem::~AISystem() {
}

void AISystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::WalkerAI*> AIVector = scene.GetAll<Component::WalkerAI>();
    for (auto AIComponent : AIVector)
        AIComponent->Update(deltaTime);
}
