#include "SpawnerSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Component/Spawner.hpp"
#include <Threading/Threading.hpp>

using namespace System;

SpawnerSystem::SpawnerSystem() {
}

SpawnerSystem::~SpawnerSystem() {
}

void SpawnerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Spawner*>& spawnerVector = scene.GetAll<Component::Spawner>();
    
    mDeltaTime = deltaTime;
    Threading::ParallelFor(&SpawnerSystem::UpdatePart, this, spawnerVector);
    Threading::FrontEndJobs().Wait();
}

void SpawnerSystem::UpdatePart(std::vector<Component::Spawner*>& spawners, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i)
        spawners[i]->timeSinceSpawn += mDeltaTime;
}
