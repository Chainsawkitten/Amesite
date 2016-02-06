#include "Enemy.hpp"

#include <Engine/Scene/Scene.hpp>
#include <Engine/Entity/Entity.hpp>

#include <Engine/Resources.hpp>
#include <Geometry/Geometry3D.hpp>
#include <Geometry/Cube.hpp>

#include "../Component/Controller.hpp"
#include "../Component/Health.hpp"
#include "../Component/Spawner.hpp"
#include <Engine/Component/Transform.hpp>
#include <Engine/Component/Mesh.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Enemy::Enemy(Scene* scene) : SuperGameObject(scene) {
    Entity* body = mScene->CreateEntity();
    mEntityMap["body"] = body;
    body->AddComponent<Component::Mesh>();
    body->AddComponent<Component::Transform>();
    
    body->AddComponent<Component::Collider2DCircle>();
    body->AddComponent<Component::Physics>();
    body->AddComponent<Component::Spawner>();
    body->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AlwaysShoot);
    body->AddComponent<Component::Health>();

    body->GetComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    body->GetComponent<Component::Collider2DCircle>()->radius = 0.5f;
    body->GetComponent<Component::Spawner>()->delay = 0.01f;
    body->GetComponent<Component::Health>()->faction = 1;
    body->GetComponent<Component::Health>()->health = 1;
    mEntityVector.push_back(body);
}

Enemy::~Enemy() {
    Resources().FreeCube();
}
