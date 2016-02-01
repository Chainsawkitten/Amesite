#include "GameEntityFactory.hpp"
#include <Entity/Entity.hpp>
#include "../GameObject/EnemyObject.hpp"
#include <Resources.hpp>
#include <Component/Mesh.hpp>
#include <Component/Transform.hpp>
#include <Component/RelativeTransform.hpp>
#include <Component/Collider2DCircle.hpp>
#include <Component/Collider2DRectangle.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>

#include <Scene/Scene.hpp>

GameObjectFactory& GameObjectFactory::GetInstance() {
    static GameObjectFactory instance;

    return instance;
}

GameObjectFactory::GameObjectFactory(){
    mScene = nullptr;
}

GameObjectFactory::GameObjectFactory(Scene* scene) {
    mScene = scene;
}

EnemyObject* GameObjectFactory::CreateBasicEnemy(glm::vec3 origin){
    EnemyObject* newEnemy = new EnemyObject();
    newEnemy->entityList.insert (std::make_pair("body", mScene->CreateEntity()));

    newEnemy->entityList["body"]->AddComponent<Component::Transform>();
    newEnemy->entityList["body"]->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    newEnemy->entityList["body"]->AddComponent<Component::Collider2DCircle>();
    newEnemy->entityList["body"]->GetComponent<Component::Transform>()->position = origin;
    newEnemy->entityList["body"]->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;
}

void GameObjectFactory::SetScene(Scene* scene) {
    mScene = scene;
}

GameObjectFactory::~GameObjectFactory() {

}

GameObjectFactory& GameEntityCreator() {
    return GameObjectFactory::GetInstance();
}
