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
#include "../Component/GridCollide.hpp"
#include "../Component/Explode.hpp"

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
#include "../GameObject/Enemy/Rocket.hpp"
#include "../GameObject/Enemy/Pylon.hpp"
#include "../GameObject/Enemy/Nest.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Dust.hpp"
#include "../GameObject/Explosion.hpp"
#include "../GameObject/Shield.hpp"
#include "../GameObject/EnemySpawner.hpp"
#include "../GameObject/Boss/SpinBoss.hpp"
#include "../GameObject/Boss/ShieldBoss.hpp"
#include "../GameObject/Boss/DivideBoss.hpp"
#include "../GameObject/Boss/RingBoss.hpp"
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

EnemySpawner* GameEntityFactory::CreateEnemySpawner(unsigned int type, float delay)
{
    EnemySpawner *gameObject = new EnemySpawner(mScene, type);
    gameObject->body->GetComponent<Component::Spawner>()->delay = delay;
    return gameObject;
}

Rocket* GameEntityFactory::CreateRocket(const glm::vec3& origin) {
    Rocket* gameObject = new Rocket(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

Rocket* GameEntityFactory::CreateMiniRocket(const glm::vec3& origin) {
    Rocket* gameObject = new Rocket(mScene);
    float scaleFactor = 0.25f;
    Component::Transform* transform = gameObject->node->GetComponent<Component::Transform>();
    transform->position = origin;
    transform->scale *= scaleFactor;
    gameObject->node->AddComponent<Component::LifeTime>()->lifeTime = 5.f;
    gameObject->node->GetComponent<Component::Damage>()->removeOnImpact = true;
    gameObject->node->GetComponent<Component::GridCollide>()->removeOnImpact = true;
    gameObject->node->GetComponent<Component::Explode>()->size *= scaleFactor;
    gameObject->body->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= scaleFactor;
    gameObject->body->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= scaleFactor;
    Component::Health *healthComp = gameObject->node->GetComponent<Component::Health>();
    healthComp->maxHealth = healthComp->health = 20.f;
    healthComp->removeOnLowHealth = true;
    return gameObject;
}

Nest* GameEntityFactory::CreateNest(const glm::vec3& origin) {
    Nest* gameObject = new Nest(mScene);
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

GameObject::ShieldBoss* GameEntityFactory::CreateShieldBoss(const glm::vec3& origin) {
    ShieldBoss* gameObject = new ShieldBoss(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::DivideBoss* GameEntityFactory::CreateDivideBoss(const glm::vec3& origin) {
    DivideBoss* gameObject = new DivideBoss(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

GameObject::RingBoss* GameEntityFactory::CreateRingBoss(const glm::vec3& origin) {
    RingBoss* gameObject = new RingBoss(mScene);
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
    Bullet* gameObject = new Bullet(mScene, 1.2f);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::Physics>()->velocity = direction;
    gameObject->node->GetComponent<Component::Physics>()->maxVelocity = glm::length(direction);
    gameObject->node->GetComponent<Component::LifeTime>()->lifeTime = 1.0f;
    gameObject->node->GetComponent<Component::Damage>()->faction = faction;
    return gameObject;
}

Bullet* GameEntityFactory::CreateEnemyBullet(const glm::vec3& position, const glm::vec3& direction, int faction) {
    Bullet* gameObject = new Bullet(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::Physics>()->velocity = 1.5f*direction;
    gameObject->node->GetComponent<Component::Physics>()->maxVelocity = 1.5f*glm::length(direction);
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
