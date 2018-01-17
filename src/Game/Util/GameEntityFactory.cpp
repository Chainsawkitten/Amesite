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
#include <Component/PointLight.hpp>
#include <Component/ParticleEmitter.hpp>
#include <Component/Material.hpp>
#include "../Component/Spawner.hpp"
#include "../Component/Controller.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Health.hpp"
#include "../Component/LifeTime.hpp"
#include "../Component/GridCollide.hpp"
#include "../Component/Explode.hpp"

#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include <Geometry/OBJModel.hpp>

#include <Texture/Texture2D.hpp>

#include <Scene/Scene.hpp>

#include <../Game/Util/ControlSchemes.hpp>
#include "../Util/CaveGenerator.hpp"

#include "../GameObject/Bullet.hpp"
#include "../GameObject/Player/SuperPlayer.hpp"
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
#include "../GameObject/ReviveCircle.hpp"

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
    gameObject->node->GetComponent<Component::Damage>()->damageAmount = 8999;
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

Entity* GameEntityFactory::CreateCrashSite1() {

    Entity* crash = mScene->CreateEntity();
    crash->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/player1/player1_body.obj");
    crash->AddComponent<Component::Material>()->SetDiffuse("Resources/player1/player1_body_diff_dead.png");
    crash->GetComponent<Component::Material>()->SetSpecular("Resources/player1/player1_body_spec.png");
    crash->GetComponent<Component::Material>()->SetGlow("Resources/player1/player1_body_glow.png");
    crash->AddComponent<Component::Transform>()->scale *= 0.25f;

    return crash;

}

Entity* GameEntityFactory::CreateCrashSite2() {

    Entity* crash = mScene->CreateEntity();
    crash->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/player2/player2_body.obj");
    crash->AddComponent<Component::Material>()->SetDiffuse("Resources/player2/player2_diff_dead.png");
    crash->GetComponent<Component::Material>()->SetSpecular("Resources/player2/player2_spec.png");
    crash->GetComponent<Component::Material>()->SetGlow("Resources/player2/player2_glow.png");
    crash->AddComponent<Component::Transform>()->scale *= 0.25f;

    return crash;

}

Entity* GameEntityFactory::CreateGlowingStone() {

    Entity* stone = mScene->CreateEntity();
    stone->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/stones/stone_01.obj");
    stone->AddComponent<Component::Material>()->SetDiffuse("Resources/stones/stone_02_diff.png");
    stone->GetComponent<Component::Material>()->SetSpecular("Resources/color/enemy_spec.png");
    stone->GetComponent<Component::Material>()->SetGlow("Resources/stones/stone_02_glow.png");
    stone->AddComponent<Component::Transform>()->scale *= 0.3f;

    Component::ParticleEmitter* emitter = stone->AddComponent<Component::ParticleEmitter>();
    emitter->emitterType = Component::ParticleEmitter::CUBOID;
    emitter->size = glm::vec3(3.f, 3.f, 3.f);
    emitter->maxEmitTime = 1.2f;
    emitter->minEmitTime = 0.8f;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 5.0;
    emitter->particleType.textureIndex = Component::ParticleEmitter::DUST;
    emitter->particleType.minLifetime = 2.f;
    emitter->particleType.maxLifetime = 3.f;
    emitter->particleType.minVelocity = glm::vec3(-.6f, 1.f, -.6f);
    emitter->particleType.maxVelocity = glm::vec3(.6f, 2.f, .6f);
    emitter->particleType.minSize = glm::vec2(.2f, .2f) * 2.f;
    emitter->particleType.maxSize = glm::vec2(.4f, .4f) * 2.f;
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::normalize(glm::vec3(109.f, 242.f, 207.f));
    emitter->particleType.startAlpha = 1.f;
    emitter->particleType.midAlpha = 1.f;
    emitter->particleType.endAlpha = 0.f;

    return stone;

}

Entity* GameEntityFactory::CreateStone() {

    Entity* stone = mScene->CreateEntity();
    stone->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/stones/stone_01.obj");
    stone->AddComponent<Component::Material>()->SetDiffuse("Resources/color/DefaultGray.png");
    stone->GetComponent<Component::Material>()->SetSpecular("Resources/color/enemy_spec.png");
    stone->AddComponent<Component::Transform>()->scale *= 0.3f;

    return stone;

}

Entity* GameEntityFactory::CreateFallenPillar() {

    Entity* pillar = mScene->CreateEntity();
    pillar->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/spawn/pillar/Pillar2.obj");
    pillar->AddComponent<Component::Material>()->SetDiffuse("Resources/spawn/pillar/Pillar_Albedo.png");
    pillar->GetComponent<Component::Material>()->SetSpecular("Resources/spawn/pillar/Pillar_NM.png");
    pillar->GetComponent<Component::Material>()->SetGlow("Resources/spawn/pillar/Pillar_Glow2.png");
    pillar->AddComponent<Component::Transform>();

    return pillar;

}

Entity* GameEntityFactory::CreateBrokenFallenPillar() {

    Entity* pillar = mScene->CreateEntity();
    pillar->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/spawn/pillar/pillar_bot.obj");
    pillar->AddComponent<Component::Material>()->SetDiffuse("Resources/spawn/pillar/Pillar_Albedo.png");
    pillar->GetComponent<Component::Material>()->SetSpecular("Resources/spawn/pillar/Pillar_NM.png");
    pillar->GetComponent<Component::Material>()->SetGlow("Resources/spawn/pillar/Pillar_Glow2.png");
    pillar->AddComponent<Component::Transform>();

    return pillar;

}

Entity* GameEntityFactory::CreateCrystalLight() {

    Entity* crystal = mScene->CreateEntity();
    crystal->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/crystal/crystal.obj");
    crystal->AddComponent<Component::Material>()->SetDiffuse("Resources/color/DefaultBlue.png");
    crystal->GetComponent<Component::Material>()->SetSpecular("Resources/color/enemy_spec.png");
    crystal->GetComponent<Component::Material>()->SetGlow("Resources/default/DefaultSpecular.png");
    crystal->AddComponent<Component::Transform>()->scale *= 0.2f;

    crystal->AddComponent<Component::PointLight>();
    crystal->GetComponent<Component::PointLight>()->attenuation = 0.8f;
    crystal->GetComponent<Component::PointLight>()->color = glm::vec3(109.f, 242.f, 207.f) * 0.01f;

    return crystal;

}

Entity* GameEntityFactory::CreateShrapnel(glm::vec3 position, unsigned int amount, Component::Explode* explodeComponent) {
    for (unsigned int i = 0; i < amount; i++) {
        Entity* shrapnel = mScene->CreateEntity();
        if (explodeComponent != nullptr) {
            switch (explodeComponent->type) {
                case Component::Explode::CAVE:
                    shrapnel->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/stones/stone_01.obj");
                    shrapnel->AddComponent<Component::Material>()->SetDiffuse("Resources/color/wall_gray.png");
                    shrapnel->GetComponent<Component::Material>()->SetSpecular("Resources/color/enemy_spec.png");
                    break;
                case Component::Explode::ENEMY:
                    shrapnel->AddComponent<Component::Mesh>()->geometry = Resources().CreateOBJModel("Resources/stones/stone_01.obj");
                    shrapnel->AddComponent<Component::Material>()->SetDiffuse("Resources/color/enemy_diff.png");
                    shrapnel->GetComponent<Component::Material>()->SetSpecular("Resources/color/enemy_spec.png");
                    shrapnel->GetComponent<Component::Material>()->SetGlow("Resources/color/enemy_glow.png");
                    break;
                default:
                    break;
            }
        }
        shrapnel->AddComponent<Component::Transform>()->scale *= 0.03f;
        shrapnel->GetComponent<Component::Transform>()->position = position;
        shrapnel->AddComponent<Component::Physics>()->gravityFactor = 5.f;
        shrapnel->GetComponent<Component::Physics>()->velocity = 20.f * glm::vec3(rand() % 20 / 10.f - 1.f, rand() % 20 / 10.f - 1.f, rand() % 20 / 10.f - 1.f);
        shrapnel->GetComponent<Component::Physics>()->maxVelocity = 10.f * glm::length(shrapnel->GetComponent<Component::Physics>()->velocity);
        shrapnel->AddComponent<Component::GridCollide>()->removeOnImpact = false;
        shrapnel->AddComponent<Component::LifeTime>()->lifeTime = 2.f;
        shrapnel->AddComponent<Component::Collider2DCircle>()->radius = 2.f;
    }
    return nullptr;
}

GameObject::PillarBall* GameEntityFactory::CreatePillarBall(const glm::vec3& origin, const glm::vec3& destination) {
    PillarBall* gameObject = new PillarBall(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    gameObject->startPosition = glm::vec2(origin.x, origin.z);
    gameObject->destination = glm::vec2(destination.x, destination.z);
    gameObject->startHeight = origin.y;
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
    gameObject->node->GetComponent<Component::LifeTime>()->lifeTime = gameObject->node->GetComponent<Component::LifeTime>()->initialLifeTime = 1.3f;
    gameObject->node->GetComponent<Component::Damage>()->faction = faction;
    gameObject->node->GetComponent<Component::PointLight>()->color = glm::vec3(0.25f, 0.96f, 0.37f);
    gameObject->lightIntensity = 0.3f;

    Component::ParticleEmitter* emitter = gameObject->node->GetComponent<Component::ParticleEmitter>();
    emitter->maxEmitTime = 0.002;
    emitter->minEmitTime = 0.0016;
    emitter->particleType.color = glm::vec3(.0f, 1.f, .0f);

    gameObject->tail->GetComponent<Component::ParticleEmitter>()->enabled = false;

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
    gameObject->node->GetComponent<Component::PointLight>()->color = glm::vec3(0.67f, 0.f, 0.72f);
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

Explosion* GameEntityFactory::CreateExplosion(glm::vec3 position, float lifeTime, float size, int particleTextureIndex, Component::Explode* explodeComponent) {
    Explosion* gameObject = new Explosion(mScene);

    if (explodeComponent != nullptr) {
        if (explodeComponent->type == Component::Explode::NONE) {
            gameObject->node->GetComponent<Component::ParticleEmitter>()->enabled = false;
            gameObject->tail->GetComponent<Component::ParticleEmitter>()->enabled = false;
            gameObject->body->GetComponent<Component::ParticleEmitter>()->enabled = false;
        }
    }

    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::LifeTime>()->lifeTime = lifeTime;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;
    gameObject->node->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = particleTextureIndex;

    gameObject->tail->GetComponent<Component::Transform>()->position = position;
    gameObject->tail->GetComponent<Component::LifeTime>()->lifeTime = lifeTime;
    gameObject->tail->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
    gameObject->tail->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;

    gameObject->body->GetComponent<Component::Transform>()->position = position;
    gameObject->body->GetComponent<Component::LifeTime>()->lifeTime = lifeTime;
    gameObject->body->GetComponent<Component::ParticleEmitter>()->particleType.minSize *= size;
    gameObject->body->GetComponent<Component::ParticleEmitter>()->particleType.maxSize *= size;

    CreateShrapnel(position, 2, explodeComponent);
    return gameObject;
}

ReviveCircle* GameEntityFactory::CreateReviveCircle(SuperPlayer* player) {
    ReviveCircle* gameObject = new ReviveCircle(mScene, player);
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
