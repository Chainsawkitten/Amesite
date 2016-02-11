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
#include "../Component/Spawner.hpp"
#include <Component/SpotLight.hpp>
#include <Component/ParticleEmitter.hpp>
#include "../Component/Controller.hpp"
#include "../Component/Damage.hpp"
#include "../Component/Health.hpp"

#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../CaveSystem/CaveSystem.hpp"

#include <Texture/Texture2D.hpp>

#include <Scene/Scene.hpp>

#include <../Game/Util/ControlSchemes.hpp>

#include "../GameObject/Bullet.hpp"
#include "../GameObject/Player.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Enemy.hpp"
#include "../GameObject/Cave.hpp"

using namespace GameObject;

GameEntityFactory& GameEntityFactory::GetInstance() {
    static GameEntityFactory instance;
    
    return instance;
}

GameEntityFactory::GameEntityFactory(){
    mScene = nullptr;
}

Enemy* GameEntityFactory::CreateBasicEnemy(const glm::vec3& origin) {
    Enemy* gameObject = new Enemy(mScene);
    gameObject->GetEntity("body")->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

Player* GameEntityFactory::CreatePlayer(const glm::vec3& origin, InputHandler::Player player) {
    Player* gameObject = new Player(mScene);
    gameObject->GetEntity("node")->GetComponent<Component::Transform>()->position = origin;
    gameObject->GetEntity("node")->GetComponent<Component::Controller>()->playerID = player;
    gameObject->GetEntity("body")->GetComponent<Component::Controller>()->playerID = player;
    //gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->playerID = player;
    //if (player == InputHandler::PLAYER_ONE) {
    //    gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::StickRotate);
    //} else {
    //    gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::ArrowKeyRotate);
    //}

    return gameObject;
}

Bullet* GameEntityFactory::CreateBullet(const glm::vec3& position, const glm::vec3& direction, int faction) {
    Bullet* gameObject = new Bullet(mScene);
    gameObject->GetEntity("body")->GetComponent<Component::Transform>()->position = position;
    gameObject->GetEntity("body")->GetComponent<Component::Physics>()->velocity = direction;
    gameObject->GetEntity("body")->GetComponent<Component::Damage>()->faction = faction;
    return gameObject;
}

Camera* GameEntityFactory::CreateCamera(const glm::vec3& origin, const glm::vec3& rotation) {
    Camera* gameObject = new Camera(mScene);
    gameObject->GetEntity("body")->GetComponent<Component::Transform>()->position = origin;
    gameObject->GetEntity("body")->GetComponent<Component::Transform>()->Rotate(rotation.x, rotation.y, rotation.z);
    return gameObject;
}

void GameEntityFactory::SetScene(Scene* scene) {
    mScene = scene;
}

void GameEntityFactory::CreateCuboidParticle(Entity * object, int particleTextureIndex) {
    object->AddComponent<Component::ParticleEmitter>();
    
    // Particle emitter.
    Component::ParticleEmitter* emitter = object->GetComponent<Component::ParticleEmitter>();
    
    emitter->emitterType = Component::ParticleEmitter::CUBOID;
    emitter->follow = object;
    emitter->maxEmitTime = 0.015;
    emitter->minEmitTime = 0.01;
    emitter->lifetime = 0.0;
    emitter->origin = glm::vec3(0.f, 5.f, 0.f);
    emitter->size = glm::vec3(40.f, 20.f, 40.f);
    emitter->relative = true;
    emitter->timeToNext = 5.0;
    
    //Particle type.
    emitter->particleType.textureIndex = particleTextureIndex;
    emitter->particleType.minLifetime = 2.f;
    emitter->particleType.maxLifetime = 4.f;
    emitter->particleType.minVelocity = glm::vec3(-0.025f, -0.01f, -0.025f);
    emitter->particleType.maxVelocity = glm::vec3(0.025f, -0.1f, 0.025f);
    emitter->particleType.minSize = glm::vec2(0.025f, 0.025f);
    emitter->particleType.maxSize = glm::vec2(0.05f, 0.05f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(1.f, 0.5f, 0.5f);
}

void GameEntityFactory::CreatePointParticle(Entity * object, int particleTextureIndex) {
    object->AddComponent<Component::ParticleEmitter>();

    // Particle emitter.
    Component::ParticleEmitter* emitter = object->GetComponent<Component::ParticleEmitter>();

    emitter->emitterType = Component::ParticleEmitter::POINT;
    emitter->maxEmitTime = 0.05;
    emitter->minEmitTime = 0.03;
    emitter->timeToNext = emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
    emitter->lifetime = 0.0;
    emitter->relative = true;
    emitter->follow = object;
    emitter->origin = glm::vec3(0.f, 0.f, 0.f);

    //Particle type.
    emitter->particleType.textureIndex = particleTextureIndex;
    emitter->particleType.minLifetime = .1f;
    emitter->particleType.maxLifetime = .2f;
    emitter->particleType.minVelocity = glm::vec3(-.3f, 0.f, -.2f);
    emitter->particleType.maxVelocity = glm::vec3(.3f, 0.f, .2f);
    emitter->particleType.minSize = glm::vec2(.5f, .5f);
    emitter->particleType.maxSize = glm::vec2(1.f, 1.f);
    emitter->particleType.uniformScaling = true;
    emitter->particleType.color = glm::vec3(1.f, 0.5f, 0.5f);
}

Cave* GameEntityFactory::CreateMap() {
    Cave* gameObject = new Cave(mScene);
    return gameObject;
}

GameEntityFactory::~GameEntityFactory() {
    
}

GameEntityFactory& GameEntityCreator() {
    return GameEntityFactory::GetInstance();
}
