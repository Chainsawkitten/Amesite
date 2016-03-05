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
    
    unsigned int threads = Threading::GetParallelCount();
    std::size_t length = spawnerVector.size() / threads;
    for (unsigned int i=0; i < threads; ++i) {
        std::size_t begin = i * length;
        if (i == threads - 1)
            length = spawnerVector.size() - begin;
        
        if (length > 0)
            Threading::FrontEndJobs().Add(std::bind(&SpawnerSystem::UpdatePart, this, std::ref(spawnerVector), deltaTime, begin, length));
    }
    
    Threading::FrontEndJobs().Wait();
}

void SpawnerSystem::UpdatePart(std::vector<Component::Spawner*>& spawners, float deltaTime, std::size_t begin, std::size_t length) {
    for (std::size_t i=begin; i < begin+length; ++i)
        spawners[i]->timeSinceSpawn += deltaTime;
}
