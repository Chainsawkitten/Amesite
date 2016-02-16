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
#include <Engine/Component/PointLight.hpp>
#include <Engine/Component/Animation.hpp>
#include <Geometry/OBJModel.hpp>

#include "../Util/ControlSchemes.hpp"

using namespace GameObject;

Player::Player(Scene* scene) : SuperGameObject(scene) {
    node = CreateEntity(scene);
    node->AddComponent<Component::Transform>()->scale = glm::vec3(0.1f, 0.1f, 0.1f);;
    node->AddComponent<Component::Controller>()->speed = 3000.f;
    node->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
    node->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
    node->AddComponent<Component::Health>();
    node->AddComponent<Component::Animation>();
    Component::Animation::AnimationClip* idleNode = node->GetComponent<Component::Animation>()->CreateAnimationClip("idle");
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.f, 0.f), 2.f, 1.f, 0, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.f, 0.1f, 0.1f), -1.f, -2.f, 1.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(0.1f, 0.1f, -0.1f), -2.f, 1.f, 2.f, 1.5f, false, true);
    idleNode->CreateKeyFrame(glm::vec3(-0.1f, 0.1f, 0.1f), 2.f, 2.f, 1.f, 1.5f, false, true);
    node->GetComponent<Component::Animation>()->Start("idle");

    body = CreateEntity(scene);
    body->AddComponent<Component::RelativeTransform>()->parentEntity = node;
    body->AddComponent<Component::Mesh>()->geometry = mShipBody = Resources().CreateOBJModel("Resources/ship.obj");
    body->AddComponent<Component::Material>();
    body->GetComponent<Component::Material>()->SetDiffuse("Resources/Albedo.png");
    body->GetComponent<Component::Material>()->SetSpecular("Resources/Specular.png");
    body->GetComponent<Component::Material>()->SetNormal("Resources/Normal.png");
    body->AddComponent<Component::Controller>();
    body->AddComponent<Component::Animation>();

//    mHead = mScene->CreateEntity();
//    mEntityMap["head"] = mHead;
//    mEntityVector.push_back(mHead);
//    mHead->AddComponent<Component::RelativeTransform>()->parentEntity = mBody;
//    mHead->GetComponent<Component::Transform>()->Move(0.f, 0.f, 1.5f);
//    mHead->GetComponent<Component::Transform>()->scale *= 2.f;
//    mHead->AddComponent<Component::Mesh>()->geometry = Resources().CreateCube();
//    mHead->AddComponent<Component::Material>();
//    mHead->AddComponent<Component::Animation>();
//    mHead->AddComponent<Component::Spawner>();
//    mHead->AddComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::ButtonShoot);
//
//    Entity* spotLight = mScene->CreateEntity();
//    mEntityMap["spotLight"] = spotLight;
//
//
////=======
//    Entity* body = mScene->CreateEntity();
//    mEntityMap["body"] = body;
//    body->AddComponent<Component::Controller>()->speed = 3000.f;
//    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Move);
//    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::AimedFire);
//    body->GetComponent<Component::Controller>()->controlSchemes.push_back(&ControlScheme::Aim);
//    body->AddComponent<Component::Transform>();
//    body->AddComponent<Component::Mesh>()->geometry = mModel = Resources().CreateOBJModel("Resources/ship.obj");
//    body->GetComponent<Component::Transform>()->scale = glm::vec3(0.1f, 0.1f, 0.1f);
//    body->AddComponent<Component::Material>();
//    body->GetComponent<Component::Material>()->SetDiffuse("Resources/Albedo.png");
//    body->GetComponent<Component::Material>()->SetSpecular("Resources/Specular.png");
//    body->GetComponent<Component::Material>()->SetNormal("Resources/Normal.png");
//    body->AddComponent<Component::Collider2DCircle>()->radius = 0.5;
//    body->AddComponent<Component::Physics>()->velocityDragFactor = 3.f;
//    body->AddComponent<Component::Health>()->removeOnLowHealth = false;
//    body->AddComponent<Component::Spawner>()->delay = 0.1f;
//    mEntityVector.push_back(body);
//
//    Entity* light = mScene->CreateEntity();
//    mEntityMap["light"] = light;
//    light->AddComponent<Component::RelativeTransform>()->Move(0, 1, 0);
//    light->GetComponent<Component::RelativeTransform>()->parentEntity = body;
//    //spotLight->GetComponent<Component::RelativeTransform>()->scale = glm::vec3(0.3f, 0.3f, 0.3f);
//    //spotLight->AddComponent<Component::Mesh>()->geometry = body->GetComponent<Component::Mesh>()->geometry;
//    //spotLight->AddComponent<Component::Material>();
//    light->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
//    light->GetComponent<Component::SpotLight>()->attenuation = 0.1f;
//    //spotLight->AddComponent<Component::Physics>();
//    //spotLight->AddComponent<Component::Controller>();
////>>>>>>> 5ba7aa37f7307c3042100ae121c50345ecebd93f
//    mEntityVector.push_back(spotLight);
//    spotLight->AddComponent<Component::RelativeTransform>()->parentEntity = mHead;
//    spotLight->AddComponent<Component::Animation>();
//    spotLight->AddComponent<Component::SpotLight>()->coneAngle = 45.f;
//    spotLight->GetComponent<Component::SpotLight>()->attenuation = 0.01f;
//    spotLight->GetComponent<Component::SpotLight>()->color.g = 0.f;
}

Player::~Player() {
    Resources().FreeOBJModel(mShipBody);
}

glm::vec3 Player::GetPosition() {
    return node->GetComponent<Component::Transform>()->GetWorldPosition();
}

float Player::GetHealth() {
    return node->GetComponent<Component::Health>()->health;
}

void Player::Shoot() {
}
