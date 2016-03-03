#include "SpawnerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

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
