#include "GameEntityFactory.hpp"
#include <Entity/Entity.hpp>
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Lens.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>
#include "../CaveSystem/CaveSystem.hpp"

#include <Scene/Scene.hpp>

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

Entity* GameEntityFactory::CreateCube( glm::vec3 origin ) {
	Entity* cubeEntity = mScene->CreateEntity();
	cubeEntity->AddComponent<Component::Mesh>();
	cubeEntity->AddComponent<Component::Transform>();

	cubeEntity->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
	cubeEntity->GetComponent<Component::Transform>()->position = origin;

	return cubeEntity;
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
