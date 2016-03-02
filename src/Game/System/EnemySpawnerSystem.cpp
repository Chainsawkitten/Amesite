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
#include "../GameObject/Player.hpp"
#include "../GameObject/Pylon.hpp"
#include "../GameObject/Enemy.hpp"

#include <vector>
#include <Util/Log.hpp>

using namespace System;

System::EnemySpawnerSystem::EnemySpawnerSystem() {
    mMaxEnemyCount = 5;
    mEnemyCount = 0;
    mSpawnerRadius = 200.f;
}

System::EnemySpawnerSystem::~EnemySpawnerSystem() {
}

void System::EnemySpawnerSystem::Update(Scene& scene, float deltaTime, const GameObject::Cave* cave, const std::vector<GameObject::Player*> *players) {
    for (int i = 0; i < mPylons.size(); i++) {
        if (mPylons[i]->node->GetComponent<Component::Health>()->health < 0.01f) {
            Component::Explode* explodeComp = mPylons[i]->node->GetComponent<Component::Explode>();
            // Create Explosion    
            if (explodeComp != nullptr)
                GameEntityCreator().CreateExplosion(mPylons[i]->node->GetComponent<Component::Transform>()->GetWorldPosition() + explodeComp->offset, explodeComp->lifeTime, explodeComp->size, explodeComp->particleTextureIndex);
            
            //Kill game object
            mPylons[i]->Kill();
            mPylons.erase(mPylons.begin() + i);
        }
    }

    mEnemyCount = mEnemies.size() + mPylons.size();

    if (mEnemyCount < mMaxEnemyCount) {
        std::vector<Component::Spawner*> spawners;
        spawners = scene.GetAll<Component::Spawner>();

        for (auto spawner : spawners) {
            if (spawner->type == Component::Spawner::ENEMY) {
                spawner->timeSinceSpawn += deltaTime;
                if (spawner->delay <= spawner->timeSinceSpawn) {
                    glm::vec3 position = FindValidPosition(cave, players);       
                    if (position.x > 0.f) {
                        spawner->timeSinceSpawn = 0.0;
                        if (spawner->enemyType == Component::Spawner::BASIC) {
                            mEnemies.push_back(GameEntityCreator().CreateBasicEnemy(position));
                        }
                        else if (spawner->enemyType == Component::Spawner::PYLON) {
                            mPylons.push_back(GameEntityCreator().CreateEnemyPylon(position));
                        }
                    }
                }
            }
        }
    }
}

glm::vec3 System::EnemySpawnerSystem::FindValidPosition(const GameObject::Cave* cave, const std::vector<GameObject::Player*> *players) const {

    glm::vec3 scale = cave->map->GetComponent<Component::Transform>()->GetWorldScale();
    glm::uvec3 size = glm::vec3(cave->mWidth, 0.f, cave->mHeight);
    glm::uvec3 position;

    glm::vec3 averagePlayerPosition = glm::vec3(0.f, 0.f, 0.f);

    for (auto player : *players) {
        glm::vec3 playerPos = player->GetPosition();

        averagePlayerPosition.x += playerPos.x;
        averagePlayerPosition.y += playerPos.y;
    }
    float factor = 1.f / static_cast<float>(players->size());

    averagePlayerPosition.x *= factor;
    averagePlayerPosition.y *= factor;  
  
    bool** map = cave->GetCaveData();

    // If we can find a valid position within a certrain amount of iterations we return it.
    for (int i = 0; i < 20; i++) {
        position = glm::uvec3(rand() % size.x, 0.f, rand() % size.z);
        if (((glm::length((glm::vec3(position)*scale) - averagePlayerPosition)) > mSpawnerRadius )
            && !(map[position.z][position.x]
                || map[position.z - 1][position.x]
                || map[position.z + 1][position.x]
                || map[position.z][position.x - 1]
                || map[position.z][position.x + 1]
                || map[position.z - 1][position.x - 1]
                || map[position.z - 1][position.x + 1]
                || map[position.z + 1][position.x - 1]
                || map[position.z + 1][position.x + 1]))
            return glm::vec3(position) * scale;
    }
    return glm::vec3(-1.f, -1.f, -1.f);
}
