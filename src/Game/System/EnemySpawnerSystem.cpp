#include "EnemySpawnerSystem.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Entity/Entity.hpp>
#include <Engine/GameObject/SuperGameObject.hpp>

#include "../Util/GameEntityFactory.hpp"

#include <vector>
#include <Util/Log.hpp>

using namespace System;

System::EnemySpawnerSystem::EnemySpawnerSystem() {
    mMaxEnemyCount = 10;

  /*  if (mEnemySpawners.empty()) {
    }*/
}

System::EnemySpawnerSystem::~EnemySpawnerSystem() {
}

void System::EnemySpawnerSystem::Update(Scene& scene, float deltaTime) {

    if (mEnemyCount < mMaxEnemyCount) {
        glm::vec3 position = FindValidPosition();

        GameEntityCreator().CreateBasicEnemy(position);
    }
}

glm::vec3 System::EnemySpawnerSystem::FindValidPosition() {
    return glm::vec3();
}
