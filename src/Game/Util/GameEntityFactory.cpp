#include "GameEntityFactory.hpp"
#include <Entity/Entity.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Lens.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>
#include <Component/Physics.hpp>
#include <Component/SpotLight.hpp>
#include <Component/ParticleEmitter.hpp>
#include "../Component/Spawner.hpp"
#include "../Component/Controller.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Health.hpp"
#include "../Component/LifeTime.hpp"

#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>

#include <Texture/Texture2D.hpp>

#include <Scene/Scene.hpp>

#include <../Game/Util/ControlSchemes.hpp>
#include "../Util/CaveGenerator.hpp"

#include "../GameObject/Bullet.hpp"
#include "../GameObject/Player/Player1.hpp"
#include "../GameObject/Player/Player2.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Enemy.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Dust.hpp"
#include "../GameObject/Explosion.hpp"
#include "../GameObject/Pylon.hpp"
#include "../GameObject/Shield.hpp"
#include "../GameObject/EnemySpawner.hpp"
#include "../GameObject/Boss/SpinBoss.hpp"
#include "../GameObject/Altar.hpp"
#include "../GameObject/Pillar.hpp"
#include "../GameObject/PillarBall.hpp"
#include "../GameObject/Portal.hpp"

using namespace GameObject;

GameEntityFactory& GameEntityFactory::GetInstance() {
    static GameEntityFactory instance;
    
    return instance;
}

GameEntityFactory::GameEntityFactory(){
    mScene = nullptr;
}

EnemySpawner * GameEntityFactory::CreateEnemySpawner(unsigned int type, float delay)
{
    EnemySpawner *gameObject = new EnemySpawner(mScene, type);
    gameObject->body->GetComponent<Component::Spawner>()->delay = delay;
    return gameObject;
}

Enemy* GameEntityFactory::CreateBasicEnemy(const glm::vec3& origin) {
    Enemy* gameObject = new Enemy(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

Pylon* GameEntityFactory::CreateEnemyPylon(const glm::vec3& origin) {
    Pylon* gameObject = new Pylon(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::Shield* GameEntityFactory::CreateShield(Entity* parentEntity, glm::vec3 offset, float lifeTime, float health) {
    Shield* gameObject = new Shield(mScene);
    gameObject->node->GetComponent<Component::RelativeTransform>()->parentEntity = parentEntity;
    gameObject->node->GetComponent<Component::RelativeTransform>()->Move(offset);
    gameObject->body->GetComponent<Component::LifeTime>()->lifeTime = lifeTime;
    gameObject->body->GetComponent<Component::Health>()->health = health;
    gameObject->body->GetComponent<Component::Health>()->maxHealth = health;
    return gameObject;
}

Player1* GameEntityFactory::CreatePlayer1(const glm::vec3& origin) {
    Player1* gameObject = new Player1(mScene);
    gameObject->SetPosition(origin);
    CreateDust(gameObject->GetNodeEntity(), Component::ParticleEmitter::DUST);
    return gameObject;
}

Player2* GameEntityFactory::CreatePlayer2(const glm::vec3& origin) {
    Player2* gameObject = new Player2(mScene);
    gameObject->SetPosition(origin);
    CreateDust(gameObject->GetNodeEntity(), Component::ParticleEmitter::DUST);
    return gameObject;
}

GameObject::SpinBoss* GameEntityFactory::CreateSpinBoss(const glm::vec3& origin) {
    SpinBoss* gameObject = new SpinBoss(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::Altar* GameEntityFactory::CreateAltar(const glm::vec3& origin) {
    Altar* gameObject = new Altar(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::Pillar* GameEntityFactory::CreatePillar(const glm::vec3& origin, glm::vec3 bossPosition) {
    Pillar* gameObject = new Pillar(mScene, bossPosition);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::PillarBall* GameEntityFactory::CreatePillarBall(const glm::vec3& origin, const glm::vec3& velocity) {
    PillarBall* gameObject = new PillarBall(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    gameObject->node->GetComponent<Component::Physics>()->velocity = velocity;
    return gameObject;
}

GameObject::Portal* GameEntityFactory::CreatePortal(const glm::vec3& origin) {
    Portal* gameObject = new Portal(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

Bullet* GameEntityFactory::CreatePlayerBullet(const glm::vec3& position, const glm::vec3& direction, int faction) {
    Bullet* gameObject = new Bullet(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::Physics>()->velocity = direction;
    gameObject->node->GetComponent<Component::Physics>()->maxVelocity = glm::length(direction);
    gameObject->node->GetComponent<Component::LifeTime>()->lifeTime = 1.2f;
    gameObject->node->GetComponent<Component::Damage>()->faction = faction;
    return gameObject;
}

Bullet* GameEntityFactory::CreateEnemyBullet(const glm::vec3& position, const glm::vec3& direction, int faction) {
    Bullet* gameObject = new Bullet(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::Physics>()->velocity = direction;
    gameObject->node->GetComponent<Component::Physics>()->maxVelocity = glm::length(direction);
    gameObject->node->GetComponent<Component::Damage>()->faction = faction;
    gameObject->tail->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = Component::ParticleEmitter::PURPLE;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= 1.6f;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= 1.9f;
    gameObject->tail->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= 1.5f;
    gameObject->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= 1.8f;
    return gameObject;
}

Camera* GameEntityFactory::CreateCamera(const glm::vec3& origin, const glm::vec3& rotation) {
    Camera* gameObject = new Camera(mScene);
    gameObject->body->GetComponent<Component::Transform>()->position = origin;
    gameObject->body->GetComponent<Component::Transform>()->Rotate(rotation.x, rotation.y, rotation.z);
    return gameObject;
}

Dust* GameEntityFactory::CreateDust(Entity * object, int particleTextureIndex) {
    Dust* gameObject = new Dust(mScene);
    gameObject->body->GetComponent<Component::RelativeTransform>()->parentEntity = object;
    gameObject->body->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = particleTextureIndex;
    return gameObject;
}

Explosion* GameEntityFactory::CreateExplosion(glm::vec3 position, float lifeTime, float size, int particleTextureIndex) {
    Explosion* gameObject = new Explosion(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::LifeTime>()->lifeTime = lifeTime;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = particleTextureIndex;
    return gameObject;
}

void GameEntityFactory::SetScene(Scene* scene) {
    mScene = scene;
}

Cave* GameEntityFactory::CreateMap(int width, int height, int seed, int percent, int iterations, int threshold, CaveGenerator::Coordinate playerPosition, std::vector<CaveGenerator::Coordinate> bossPositions) {
    Cave* gameObject = new Cave(mScene, width, height, seed, percent, iterations, threshold, playerPosition, bossPositions);
    return gameObject;
}

GameEntityFactory::~GameEntityFactory() {
    
}

GameEntityFactory& GameEntityCreator() {
    return GameEntityFactory::GetInstance();
}
