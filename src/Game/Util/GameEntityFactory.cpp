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
#include "../GameObject/Dust.hpp"

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
    gameObject->GetEntity("body")->GetComponent<Component::Transform>()->position = origin;
    gameObject->GetEntity("body")->GetComponent<Component::Controller>()->playerID = player;
    gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->playerID = player;
    if (player == InputHandler::PLAYER_ONE) {
        gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::StickRotate);
    } else {
        gameObject->GetEntity("spotLight")->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::ArrowKeyRotate);
    }

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

void GameEntityFactory::CreateCuboidDust(Entity * object, int particleTextureIndex) {  
    Dust* gameObject = new Dust(mScene);

    gameObject->GetEntity("body")->GetComponent<Component::ParticleEmitter>()->follow = object;
    gameObject->GetEntity("body")->GetComponent<Component::ParticleEmitter>()->particleType.textureIndex = particleTextureIndex;
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
