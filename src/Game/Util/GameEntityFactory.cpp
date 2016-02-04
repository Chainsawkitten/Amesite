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

#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../CaveSystem/CaveSystem.hpp"

#include <Scene/Scene.hpp>
#include "../Component/Controller.hpp"

GameEntityFactory& GameEntityFactory::GetInstance() {
    static GameEntityFactory instance;

    return instance;
}

GameEntityFactory::GameEntityFactory(){
    mScene = nullptr;
}

Entity* GameEntityFactory::CreateBasicEnemy( glm::vec3 origin ) {
    Entity* enemyEntity = mScene->CreateEntity();
    enemyEntity->AddComponent<Component::Mesh>();
    enemyEntity->AddComponent<Component::Transform>();
    enemyEntity->AddComponent<Component::Collider2DCircle>();

    enemyEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    enemyEntity->GetComponent<Component::Transform>()->position = origin;
    enemyEntity->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;

    return enemyEntity;
}

Entity* GameEntityFactory::CreatePlayer(glm::vec3 origin, InputHandler::Player player) {
    
    Entity* playerEntity = mScene->CreateEntity();
    playerEntity->AddComponent<Component::Mesh>();
    playerEntity->AddComponent<Component::Transform>();
    playerEntity->AddComponent<Component::Collider2DCircle>();
    playerEntity->AddComponent<Component::Physics>();
    playerEntity->AddComponent<Component::Controller>();
    playerEntity->AddComponent<Component::Spawner>();

    playerEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    playerEntity->GetComponent<Component::Transform>()->position = origin;
    playerEntity->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;
    playerEntity->GetComponent<Component::Controller>()->playerID = player;
    playerEntity->GetComponent<Component::Spawner>()->delay = 1.f;

    //playerEntity->AddComponent<Component::Player>();

    return playerEntity;
}

Entity* GameEntityFactory::CreateCube( glm::vec3 origin ) {
	Entity* cubeEntity = mScene->CreateEntity();
	cubeEntity->AddComponent<Component::Mesh>();
	cubeEntity->AddComponent<Component::Transform>();

	cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
	cubeEntity->GetComponent<Component::Transform>()->position = origin;

	return cubeEntity;
}

Entity* GameEntityFactory::CreateBullet(glm::vec3 position, glm::vec3 direction) {
    Entity* bullet = mScene->CreateEntity();
    Component::Transform* transform = bullet->AddComponent<Component::Transform>();
    transform->position = position;
    
    Component::Physics* physics = bullet->AddComponent<Component::Physics>();
    physics->velocity = direction;
    
    Component::Mesh* mesh = bullet->AddComponent<Component::Mesh>();
    mesh->geometry = Resources().CreateCube();
    
    return bullet;
}

Entity* GameEntityFactory::CreateCamera( glm::vec3 origin, glm::vec3 rotation ) {
    Entity* cameraEntity = mScene->CreateEntity();
    cameraEntity->AddComponent<Component::Transform>();
    cameraEntity->AddComponent<Component::Lens>();

    cameraEntity->GetComponent<Component::Transform>()->position = origin;
    cameraEntity->GetComponent<Component::Transform>()->Rotate( rotation.x, rotation.y, rotation.z );

    return cameraEntity;
}

void GameEntityFactory::SetScene( Scene* scene ) {
    mScene = scene;
}

Entity* GameEntityFactory::CreateMap() {
    Caves::CaveSystem cave = Caves::CaveSystem();
    return cave.GenerateCaveSystem(mScene);
}

GameEntityFactory::~GameEntityFactory() {

}

GameEntityFactory& GameEntityCreator() {
    return GameEntityFactory::GetInstance();
}
