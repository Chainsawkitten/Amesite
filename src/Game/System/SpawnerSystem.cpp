#include "SpawnerSystem.hpp"

#include <Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <GameObject/SuperGameObject.hpp>

#include "../Component/Spawner.hpp"

#include <vector>

using namespace System;

SpawnerSystem::SpawnerSystem() {
}

SpawnerSystem::~SpawnerSystem() {
}

void SpawnerSystem::Update(Scene& scene, float deltaTime) {
    std::vector<Component::Spawner*> spawnerVector = scene.GetAll<Component::Spawner>();
    for (auto& spawnerComponent : spawnerVector)
        spawnerComponent->timeSinceSpawn += deltaTime;
}
