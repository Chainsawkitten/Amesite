#include "AISystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>

#include "../Component/AI/WalkerAI.hpp"
#include "../Component/AI/LookerAI.hpp"

using namespace System;

AISystem::AISystem() {
}

AISystem::~AISystem() {
}

void AISystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::WalkerAI*> walkers = scene.GetAll<Component::WalkerAI>();
    for (auto AIComponent : walkers)
        AIComponent->Update(deltaTime);

    std::vector<Component::LookerAI*> lookers = scene.GetAll<Component::LookerAI>();
    for (auto AIComponent : lookers)
        AIComponent->Update(deltaTime);

}
