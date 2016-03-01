#include "EnemySpawnerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Spawner.hpp"

#include "../GameObject/Cave.hpp"

#include <vector>
#include <Util/Log.hpp>

using namespace System;

System::EnemySpawnerSystem::EnemySpawnerSystem() {
    mMaxEnemyCount = 10;
    mEnemyCount = 0;
}

System::EnemySpawnerSystem::~EnemySpawnerSystem() {
}

void System::EnemySpawnerSystem::Update(Scene& scene, float deltaTime, GameObject::Cave* cave) {
    if (mEnemyCount < mMaxEnemyCount) {
        std::vector<Component::Spawner*> spawners;
        spawners = scene.GetAll<Component::Spawner>();

        for (auto spawner : spawners) {
            if (spawner->type == Component::Spawner::ENEMY) {
                spawner->timeSinceSpawn += deltaTime;
                if (spawner->delay <= spawner->timeSinceSpawn) {
                    spawner->timeSinceSpawn = 0.0;
                    glm::vec3 position = FindValidPosition(cave);
                    if (spawner->enemyType == Component::Spawner::BASIC) {
                        GameEntityCreator().CreateBasicEnemy(position);
                        mEnemyCount++;
                    }
                    else if (spawner->enemyType == Component::Spawner::PYLON) {
                        GameEntityCreator().CreateEnemyPylon(position);
                        mEnemyCount++;
                    }
                }
            }
        }
    }
}

glm::vec3 System::EnemySpawnerSystem::FindValidPosition(GameObject::Cave* cave) {

    glm::vec3 scale = cave->map->GetComponent<Component::Transform>()->GetWorldScale();
    glm::uvec3 size = glm::vec3(cave->mWidth, 0.f, cave->mHeight);
    glm::uvec3 position;
    
    bool** map = cave->GetCaveData();

    do {
        position = glm::uvec3(rand() % size.x, 0.f, rand() % size.z);
    } while (map[position.z][position.x] || map[position.z - 1][position.x] || map[position.z + 1][position.x]
        || map[position.z][position.x - 1] || map[position.z][position.z + 1]);

    return glm::vec3(position) * scale;
}
