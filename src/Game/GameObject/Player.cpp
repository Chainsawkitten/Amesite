#include "Player.hpp"

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
#include <Engine/Component/Material.hpp>
#include <Engine/Component/Physics.hpp>
#include <Engine/Component/Collider2DCircle.hpp>
#include <Engine/Component/SpotLight.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Player::Player(Scene* scene) : SuperGameObject(scene) {
    Entity* body = mScene->CreateEntity();
    mEntityMap["body"] = body;
    body->AddComponent<Component::Controller>()->speed = 3000.f;
    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::MouseAim);
    body->AddComponent<Component::Transform>();
    body->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
    body->AddComponent<Component::Material>();
    body->AddComponent<Component::Collider2DCircle>()->radius = 0.5;
    body->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    body->AddComponent<Component::Health>()->removeOnLowHealth = false;
    body->AddComponent<Component::Spawner>()->delay = 0.1f;
    mEntityVector.push_back(body);

    Entity* spotLight = mScene->CreateEntity();
    mEntityMap["spotLight"] = spotLight;
    spotLight->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
    spotLight->GetComponent<Component::RelativeTransform>()->parentEntity = body;
    spotLight->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
    spotLight->AddComponent<Component::Mesh>()->geometry = body->GetComponent<Component::Mesh>()->geometry;
    spotLight->AddComponent<Component::Material>();
    spotLight->AddComponent<Component::SpotLight>()->coneAngle = 90;
    spotLight->GetComponent<Component::SpotLight>()->attenuation = 0.1f;
    spotLight->AddComponent<Component::Physics>();
    spotLight->AddComponent<Component::Controller>();
    mEntityVector.push_back(spotLight);
}

Player::~Player() {
    Resources().FreeCube();
}
