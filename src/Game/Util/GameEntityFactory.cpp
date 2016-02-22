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

#include "../GameObject/Bullet.hpp"
#include "../GameObject/Player.hpp"
#include "../GameObject/Camera.hpp"
#include "../GameObject/Enemy.hpp"
#include "../GameObject/Cave.hpp"
#include "../GameObject/Dust.hpp"
#include "../GameObject/Explosion.hpp"

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
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    return gameObject;
}

Player* GameEntityFactory::CreatePlayer(const glm::vec3& origin, InputHandler::Player player) {
    Player* gameObject = new Player(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = origin;
    gameObject->node->GetComponent<Component::Controller>()->playerID = player;
    gameObject->leftTurrent->GetComponent<Component::Controller>()->playerID = player;
    gameObject->rightTurrent->GetComponent<Component::Controller>()->playerID = player;
    CreateDust(gameObject->node, Component::ParticleEmitter::DUST);
    if (player == InputHandler::PLAYER_ONE) {
        gameObject->node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Aim);
    } else {
        gameObject->node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::MouseRotate);
    }
    return gameObject;
}

Bullet* GameEntityFactory::CreatePlayerBullet(const glm::vec3& position, const glm::vec3& direction, int faction) {
    Bullet* gameObject = new Bullet(mScene);
    gameObject->node->GetComponent<Component::Transform>()->position = position;
    gameObject->node->GetComponent<Component::Physics>()->velocity = direction;
    gameObject->node->GetComponent<Component::Physics>()->maxVelocity = glm::length(direction);
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

Cave* GameEntityFactory::CreateMap() {
    Cave* gameObject = new Cave(mScene);
    return gameObject;
}

GameEntityFactory::~GameEntityFactory() {
    
}

GameEntityFactory& GameEntityCreator() {
    return GameEntityFactory::GetInstance();
}
