#include "AISystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/AI/WalkerAI.hpp"
#include "../Component/AI/LookerAI.hpp"

#include <vector>

using namespace System;

AISystem::AISystem() {
}

AISystem::~AISystem() {
}

void AISystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::WalkerAI*> WalkerVector = scene.GetAll<Component::WalkerAI>();
    for (auto AIComponent : WalkerVector)
        AIComponent->Update(deltaTime);

    std::vector<Component::LookerAI*> LookerVector = scene.GetAll<Component::LookerAI>();
    for (auto AIComponent : LookerVector)
        AIComponent->Update(deltaTime);

}
