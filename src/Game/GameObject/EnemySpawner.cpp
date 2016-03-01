#include "EnemySpawner.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Spawner.hpp"
#include "Enemy.hpp"

using namespace GameObject;

EnemySpawner::EnemySpawner(Scene* scene, unsigned int type) : SuperGameObject(scene) {
    body = CreateEntity(scene);
    body->AddComponent<Component::Spawner>();
    body->GetComponent<Component::Spawner>()->type = Component::Spawner::SpawnerType::ENEMY;
    body->GetComponent<Component::Spawner>()->enemyType = (Component::Spawner::EnemyType)type;
}

EnemySpawner::~EnemySpawner() {
}