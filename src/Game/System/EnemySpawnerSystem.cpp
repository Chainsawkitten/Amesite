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
}

System::EnemySpawnerSystem::~EnemySpawnerSystem() {
}

void System::EnemySpawnerSystem::Update(Scene& scene, float deltaTime, GameObject::Cave* cave) {
    /*std::vector<GameObject::Enemy*>* gameObjects = scene.GetVector<GameObject::Enemy>();
    mEnemyCount = gameObjects->size();*/

    if (mEnemyCount < mMaxEnemyCount) {
        std::vector<Component::Spawner*> spawners;
        spawners = scene.GetAll<Component::Spawner>();

        for (auto spawner : spawners) {
            if (spawner->type == Component::Spawner::ENEMY) {
                glm::vec3 position = FindValidPosition(cave);
                if (spawner->enemyType == Component::Spawner::BASIC) {
                    GameEntityCreator().CreateBasicEnemy(position);
                    mEnemyCount++;
                } else if (spawner->enemyType == Component::Spawner::PYLON) {
                    GameEntityCreator().CreateEnemyPylon(position);
                    mEnemyCount++;
                }
                
            }
        }


    }
}

glm::vec3 System::EnemySpawnerSystem::FindValidPosition(GameObject::Cave* cave) {

    return glm::vec3(0.f, 0.f, 0.f);
}
