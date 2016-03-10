#include "EnemySpawnerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>
#include <Engine/Component/Transform.hpp>

#include "../Util/GameEntityFactory.hpp"

#include "../Component/Spawner.hpp"
#include "../Component/Health.hpp"
#include "../Component/Explode.hpp"

#include "../GameObject/Cave.hpp"
#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"
#include "../GameObject/Enemy/SuperEnemy.hpp"
#include "../GameObject/Enemy/Pylon.hpp"
#include "../GameObject/Enemy/Rocket.hpp"
#include "../GameObject/Enemy/Nest.hpp"
#include "../GameObject/Boss/SpinBoss.hpp"

#include <vector>
#include <Util/Log.hpp>

using namespace System;

EnemySpawnerSystem::EnemySpawnerSystem() {
    mMinEnemyCount = 15;
    mMaxEnemyCount = 30;
    mEnemyCount = 0;
    mEnemiesKilled = 0;
    mSpawnerRadius = 60.f;
}

EnemySpawnerSystem::~EnemySpawnerSystem() {
}

void EnemySpawnerSystem::Update(Scene& scene, float deltaTime, const GameObject::Cave* cave, const std::vector<GameObject::SuperPlayer*> *players, const std::vector<glm::vec3> noSpawnRooms) {
    for (unsigned int i = 0; i < mEnemies.size(); i++) {
        if (mEnemies[i]->GetHealth() < 0.01f) {
            mEnemies[i]->Kill();
            mEnemiesKilled++;
            mEnemies.erase(mEnemies.begin() + i);
        }
    }

    if (mEnemies.size() < mMaxEnemyCount) {
        std::vector<Component::Spawner*> spawners;
        spawners = scene.GetAll<Component::Spawner>();

        for (auto spawner : spawners) {
            if (spawner->type == Component::Spawner::ENEMY) {
                if (mEnemyCount < mMinEnemyCount) {
                    spawner->timeSinceSpawn += (deltaTime * 20);
                }
                if (spawner->delay <= spawner->timeSinceSpawn) {
                    glm::vec3 position = FindValidPosition(cave, players, noSpawnRooms);
                    if (position.x > 0.f) {
                        spawner->timeSinceSpawn = 0.0;
                        if (spawner->enemyType == Component::Spawner::PYLON) {
                            mEnemies.push_back(GameEntityCreator().CreateEnemyPylon(position));
                        } else if (spawner->enemyType == Component::Spawner::ROCKET) {
                            mEnemies.push_back(GameEntityCreator().CreateRocket(position));
                        } else if (spawner->enemyType == Component::Spawner::NEST)
                            mEnemies.push_back(GameEntityCreator().CreateNest(position));
                    }
                }
            }
        }
    }
}

unsigned int EnemySpawnerSystem::GetEnemiesKilled() {
    return mEnemiesKilled;
}

const std::vector<GameObject::SuperEnemy*>& EnemySpawnerSystem::GetEnemies() const {
    return mEnemies;
}

glm::vec3 EnemySpawnerSystem::FindValidPosition(const GameObject::Cave* cave, const std::vector<GameObject::SuperPlayer*> *players, const std::vector<glm::vec3> noSpawnRooms) const {

    glm::vec3 mMapScale = cave->map->GetComponent<Component::Transform>()->GetWorldScale();
    glm::uvec3 size = glm::vec3(cave->GetWidth(), 0.f, cave->GetHeight());
    glm::uvec3 position;

    int bossRadius = cave->GetBossRoomRadius();

    glm::vec3 averagePlayerPosition = glm::vec3(0.f, 0.f, 0.f);

    for (auto player : *players) {
        glm::vec3 playerPos = player->GetPosition();

        averagePlayerPosition.x += playerPos.x;
        averagePlayerPosition.z += playerPos.z;
    }
    float factor = 1.f / static_cast<float>(players->size());

    averagePlayerPosition.x *= factor;
    averagePlayerPosition.z *= factor;
  
    bool** map = cave->GetCaveData();

    // If we can find a valid position within a certrain amount of iterations we return it.
    for (int i = 0; i < 20; i++) {
        position = glm::uvec3(rand() % size.x, 0.f, rand() % size.z);
        if (((glm::length((glm::vec3(position)*mMapScale) - averagePlayerPosition)) > mSpawnerRadius )
            && !(map[position.z][position.x]
                || map[position.z - 1][position.x]
                || map[position.z + 1][position.x]
                || map[position.z][position.x - 1]
                || map[position.z][position.x + 1]
                || map[position.z - 1][position.x - 1]
                || map[position.z - 1][position.x + 1]
                || map[position.z + 1][position.x - 1]
                || map[position.z + 1][position.x + 1])
            && !ProximityToBosses(position, noSpawnRooms, bossRadius) )
            return glm::vec3(position) * mMapScale;
    }
    return glm::vec3(-1.f, -1.f, -1.f);
}

bool EnemySpawnerSystem::ProximityToBosses(const glm::vec3 position, const std::vector<glm::vec3> noSpawnRooms, int bossRadius) const {
    for (auto room : noSpawnRooms) {
        if (glm::length(position - room) > (float)bossRadius + 2.f)
            return false;
    }
    return true;
}
