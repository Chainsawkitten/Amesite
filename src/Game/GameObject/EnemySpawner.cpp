#include "EnemySpawner.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include "../Component/Spawner.hpp"
#include "Enemy.hpp"

using namespace GameObject;

EnemySpawner::EnemySpawner(Scene* scene, Enemy::EnemyType type) : SuperGameObject(scene) {
    body = CreateEntity(scene);
    body->AddComponent<Component::Spawner>();
    mType = type;
}

EnemySpawner::~EnemySpawner() {
}